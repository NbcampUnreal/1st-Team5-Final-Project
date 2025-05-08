// PayRockGames

#include "PRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/Player/PRPlayerController.h"
#include "PayRock/UI/HUD/BaseHUD.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"

APRCharacter::APRCharacter()
{
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;


    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true;
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;
    GetMesh()->SetUsingAbsoluteRotation(false);
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f)); // SkeletalMesh 기본 정렬 보정

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    CrouchSpeed = 300.0f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    BackwardSpeedMultiplier = 0.5f;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    MouseSensitivity = 1.0f;

    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    GetCharacterMovement()->CrouchedHalfHeight = 60.f; 

    SetupStimuliSource();
}

void APRCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitAbilityActorInfo();
}

void APRCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    InitAbilityActorInfo();
}

int32 APRCharacter::GetCharacterLevel()
{
    const APRPlayerState* PRPlayerState = GetPlayerState<APRPlayerState>();
    check(PRPlayerState);
    return PRPlayerState->GetCharacterLevel();
}

void APRCharacter::InitAbilityActorInfo()
{
    APRPlayerState* PRPlayerState = GetPlayerState<APRPlayerState>();
    check(PRPlayerState);
    AbilitySystemComponent = PRPlayerState->GetAbilitySystemComponent();
    AbilitySystemComponent->InitAbilityActorInfo(PRPlayerState, this);
    Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoInitialized();
    AttributeSet = PRPlayerState->GetAttributeSet();

    if (APRPlayerController* PC = GetController<APRPlayerController>())
    {
        if (ABaseHUD* HUD = PC->GetHUD<ABaseHUD>())
        {
            HUD->InitOverlay(PC, GetPlayerState(), AbilitySystemComponent, AttributeSet);
        }
    }
    InitializeDefaultAttributes();
}

void APRCharacter::SetupStimuliSource()
{
    StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
    StimuliSourceComponent->bAutoRegister = true;

    TArray<TSubclassOf<UAISense>> Senses = {
        UAISense_Sight::StaticClass(),
        UAISense_Damage::StaticClass()
    };

    for (auto Sense : Senses)
    {
        StimuliSourceComponent->RegisterForSense(Sense);
    }
}

void APRCharacter::SetSpeed(float NewSpeedMultiplier)
{
    float SpeedMultiplier = FMath::Clamp(NewSpeedMultiplier, 0.1f, 1.0f); // 속도가 너무 작아지지 않도록 제한

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SpeedMultiplier;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier * SpeedMultiplier;
    GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed * SpeedMultiplier;

    UE_LOG(LogTemp, Warning, TEXT("Speed Updated -> Walk: %f | Sprint: %f"),
        GetCharacterMovement()->MaxWalkSpeed, SprintSpeed);
}

void APRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (APRPlayerController* PlayerController = Cast<APRPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // 이동
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                // 점프 (Space)
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartJump
                );

                // 점프 멈추기
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                // 시선
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                // 스프린트
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartSprint
                );
                // 다시 걷기
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopSprint
                );
            }

            if (PlayerController->CrouchAction)
            {
                // 앉기
                EnhancedInput->BindAction(
                    PlayerController->CrouchAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartCrouch
                );
                // 앉기 멈추고 일어서기
                EnhancedInput->BindAction(
                    PlayerController->CrouchAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopCrouch
                );
            }

            if (PlayerController->AttackAction)
            {
                // 공격
                EnhancedInput->BindAction(
                    PlayerController->AttackAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartAttack
                );
                EnhancedInput->BindAction(
                    PlayerController->AttackAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopAttack
                );
            }

            if (PlayerController->InteractAction)
            {
                //상호작용
                EnhancedInput->BindAction(
                    PlayerController->InteractAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::Interact
                );
            }
        }
    }
}

void APRCharacter::Move(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();
    if (MoveInput.IsNearlyZero()) return;

    // 캐릭터가 바라보는 방향 기준 회전
    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

    const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    // 전체 이동 방향 계산
    FVector MoveDir = Forward * MoveInput.Y + Right * MoveInput.X;
    MoveDir.Normalize(); // 대각선 정규화

    // 뒤로가는 입력일 경우 속도 감속
    float SpeedMultiplier = (MoveInput.Y < 0.f) ? BackwardSpeedMultiplier : 1.0f;

    AddMovementInput(MoveDir, SpeedMultiplier);
}
void APRCharacter::StartJump(const FInputActionValue& value)
{
    if (GetCharacterMovement()->IsCrouching())
    {
        return;
    }

    Jump();
}

void APRCharacter::StopJump(const FInputActionValue& value)
{
    StopJumping();
}

void APRCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X * MouseSensitivity);
    AddControllerPitchInput(LookInput.Y * MouseSensitivity);
}

void APRCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void APRCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void APRCharacter::StartCrouch(const FInputActionValue& value)
{
    if (GetCharacterMovement()->NavAgentProps.bCanCrouch)
    {
        Crouch();
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
    }
}

void APRCharacter::StopCrouch(const FInputActionValue& value)
{
    if (GetCharacterMovement()->NavAgentProps.bCanCrouch)
    {
        UnCrouch();
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void APRCharacter::StartAttack(const FInputActionValue& value)
{
}

void APRCharacter::StopAttack(const FInputActionValue& value)
{
}

void APRCharacter::StartGuard(const FInputActionValue& value)
{
}

void APRCharacter::StopGuard(const FInputActionValue& value)
{
}

void APRCharacter::Interact(const FInputActionValue& value)
{
}