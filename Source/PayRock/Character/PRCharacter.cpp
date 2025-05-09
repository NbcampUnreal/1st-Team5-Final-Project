// PayRockGames

#include "PRCharacter.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Input/PRInputComponent.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/Player/PRPlayerController.h"
#include "PayRock/UI/HUD/BaseHUD.h"
#include "PayRock/Interface/PRInterface.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"

APRCharacter::APRCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

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
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f)); // SkeletalMesh �⺻ ���� ����

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    CrouchSpeed = 300.0f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    BackwardSpeedMultiplier = 0.5f;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    MouseSensitivity = 1.0f;

    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    GetCharacterMovement()->SetCrouchedHalfHeight(60.f); 

    SetupStimuliSource();

    bReplicates = true;
}

void APRCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();
	AddCharacterAbilities();
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

void APRCharacter::AddCharacterAbilities()
{
	UPRAbilitySystemComponent* PR_ASC = CastChecked<UPRAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	PR_ASC->AddCharacterAbilities(DefaultAbilities);
	PR_ASC->AddCharacterPassiveAbilities(DefaultPassiveAbilities);
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
    
    StimuliSourceComponent->RegisterWithPerceptionSystem();
}

void APRCharacter::SetSpeed(float NewSpeedMultiplier)
{
    float SpeedMultiplier = FMath::Clamp(NewSpeedMultiplier, 0.1f, 1.0f); // �ӵ��� �ʹ� �۾����� �ʵ��� ����

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SpeedMultiplier;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier * SpeedMultiplier;
    GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed * SpeedMultiplier;

    UE_LOG(LogTemp, Warning, TEXT("Speed Updated -> Walk: %f | Sprint: %f"),
        GetCharacterMovement()->MaxWalkSpeed, SprintSpeed);
}

void APRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (APRPlayerController* PlayerController = Cast<APRPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // �̵�
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                // ���� (Space)
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartJump
                );

                // ���� ���߱�
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                // �ü�
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                // ������Ʈ
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartSprint
                );
                // �ٽ� �ȱ�
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopSprint
                );
            }

            if (PlayerController->CrouchAction)
            {
                // �ɱ�
                EnhancedInput->BindAction(
                    PlayerController->CrouchAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartCrouch
                );
                // �ɱ� ���߰� �Ͼ��
                EnhancedInput->BindAction(
                    PlayerController->CrouchAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopCrouch
                );
            }

            if (PlayerController->AttackAction)
            {
                // ����
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
                EnhancedInput->BindAction(
                    PlayerController->InteractAction,
                    ETriggerEvent::Started,
                    this,
                    &APRCharacter::Interact
                );
            }
        }
    }

    if (UPRInputComponent* PRInputComponent = Cast<UPRInputComponent>(PlayerInputComponent))
    {
        PRInputComponent->BindAbilityActions(InputConfig, this,
            &APRCharacter::AbilityInputTagPressed,
            &APRCharacter::AbilityInputTagReleased,
            &APRCharacter::AbilityInputTagHeld);
    }
}

void APRCharacter::Move(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();
    if (MoveInput.IsNearlyZero()) return;

    // ĳ���Ͱ� �ٶ󺸴� ���� ���� ȸ��
    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

    const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    // ��ü �̵� ���� ���
    FVector MoveDir = Forward * MoveInput.Y + Right * MoveInput.X;
    MoveDir.Normalize(); // �밢�� ����ȭ

    // �ڷΰ��� �Է��� ��� �ӵ� ����
    float SpeedMultiplier = (MoveInput.Y < 0.f) ? BackwardSpeedMultiplier : 1.0f;

    AddMovementInput(MoveDir, SpeedMultiplier);
}

void APRCharacter::StartJump(const FInputActionValue& value)
{
    if (GetCharacterMovement()->IsCrouching()) return;

    Jump();
    bJustJumped = true;
    bResetJustJumpedNextFrame = true;
    JustJumpedElapsedTime = 0.f; // 타이머 초기화
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

AActor* APRCharacter::FindInteractableActor() const
{
    // 캡슐 기준 위치에서 시작
    FVector Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0.f, 0.f, BaseEyeHeight); // 눈높이 정도 보정
    FVector Direction = GetActorForwardVector();
    FVector End = Start + (Direction * InteractionDistance);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredComponent(GetMesh());

    FHitResult Hit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 1.f, 0, 1.f);

    if (bHit && Hit.GetActor())
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());

        if (Hit.GetActor()->Implements<UPRInterface>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Interactable interface detected!"));
            return Hit.GetActor();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor has no PRInterface"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No actor hit by line trace"));
    }

    return nullptr;
}

void APRCharacter::Interact(const FInputActionValue& value)
{
    UE_LOG(LogTemp, Warning, TEXT("Interact key pressed"));

    AActor* Target = FindInteractableActor();
    if (!Target)
    {
        UE_LOG(LogTemp, Warning, TEXT("No interactable actor found"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *Target->GetName());

    if (IPRInterface* Interactable = Cast<IPRInterface>(Target))
    {
        if (Interactable->CanInteract(this))
        {
            UE_LOG(LogTemp, Warning, TEXT("Can interact with: %s"), *Target->GetName());
            Interactable->Interact(this);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot interact (CanInteract() returned false)"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Target does not implement IPRInterface"));
    }
}

void APRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APRCharacter, MoveDirection);
    DOREPLIFETIME(APRCharacter, bIsSprinting);
    DOREPLIFETIME(APRCharacter, bIsCrouching);
    DOREPLIFETIME(APRCharacter, bIsInAir);
    DOREPLIFETIME(APRCharacter, bIsAttacking);
    DOREPLIFETIME(APRCharacter, bIsGuarding);
}

void APRCharacter::OnRep_MoveDirection()
{
}

void APRCharacter::OnRep_Sprinting() 
{
}

void APRCharacter::OnRep_Crouching() 
{
}

void APRCharacter::OnRep_InAir() 
{
}

void APRCharacter::OnRep_Attacking() 
{
}

void APRCharacter::OnRep_Guarding() 
{
}

void APRCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    const FVector Velocity = GetVelocity();
    const FRotator ActorRot = GetActorRotation();

    MoveDirection = CalculateDirectionCustom(Velocity, ActorRot);
    bIsInAir = GetCharacterMovement()->IsFalling();
    bIsCrouching = GetCharacterMovement()->IsCrouching();

    // 다음 프레임에 자동 리셋
    if (bResetJustJumpedNextFrame)
    {
        JustJumpedElapsedTime += DeltaSeconds;

        if (JustJumpedElapsedTime >= 0.2f)
        {
            bJustJumped = false;
            bResetJustJumpedNextFrame = false;
            JustJumpedElapsedTime = 0.f;
        }
    }
}

float APRCharacter::CalculateDirectionCustom(const FVector& Velocity, const FRotator& BaseRotation)
{
    if (Velocity.IsNearlyZero())
    {
        return 0.f;
    }

    const FVector Forward = FRotationMatrix(BaseRotation).GetUnitAxis(EAxis::X);
    const FVector Right = FRotationMatrix(BaseRotation).GetUnitAxis(EAxis::Y);
    const FVector NormalizedVel = Velocity.GetSafeNormal2D();

    const float ForwardDot = FVector::DotProduct(Forward, NormalizedVel);
    const float RightDot = FVector::DotProduct(Right, NormalizedVel);

    const float Angle = FMath::RadiansToDegrees(FMath::Atan2(RightDot, ForwardDot));
    return Angle;
}

void APRCharacter::SetJustJumped(bool bNewValue)
{
    bJustJumped = bNewValue;
}

void APRCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void APRCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
    UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent);
    if (!ASC) return;
    ASC->AbilityInputTagReleased(InputTag);}

void APRCharacter::AbilityInputTagHeld(FGameplayTag InputTag)
{
    UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent);
    if (!ASC) return;
    ASC->AbilityInputTagHeld(InputTag);
}
