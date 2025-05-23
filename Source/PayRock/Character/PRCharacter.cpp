// PayRockGames

#include "PRCharacter.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Blessing/BlessingComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/UserDefinedEnum.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
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
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f)); // SkeletalMesh

    DefaultArmLength = 300.f;
    AimingArmLength = 150.f;
    CameraInterpSpeed = 10.f;

    RightHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
    LeftHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("LefttHandCollision"));
    Weapon2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon2"));
    Weapon2->SetupAttachment(GetMesh(), Weapon2SocketName);
    Weapon2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BlessingComponent = CreateDefaultSubobject<UBlessingComponent>(TEXT("BlessingComponent"));

    NormalSpeed = 350.0f;
    SprintSpeedMultiplier = 2.0f;
    CrouchSpeed = 200.0f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    CurrentTargetSpeed = NormalSpeed;
    ReplicatedMaxWalkSpeed = NormalSpeed;
    BackwardSpeedMultiplier = 1.0f;
    AimingSpeedMultiplier = 0.75f;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    MouseSensitivity = 1.0f;

    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    GetCharacterMovement()->SetCrouchedHalfHeight(60.f);

    SetupStimuliSource();

    bReplicates = true;
    SetReplicateMovement(true);
}

void APRCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (SpringArmComp)
    {
        DefaultArmLength = SpringArmComp->TargetArmLength;
        DefaultSocketOffset = SpringArmComp->SocketOffset;
    }

    RightHandCollisionComp->AttachToComponent(
        GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandSocketName);
    RightHandCollisionComp->SetRelativeLocation(FVector::ZeroVector);
    RightHandCollisionComp->SetSphereRadius(10.f);
    RightHandCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    RightHandCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    RightHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    RightHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    RightHandCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    LeftHandCollisionComp->AttachToComponent(
        GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandSocketName);
    LeftHandCollisionComp->SetRelativeLocation(FVector::ZeroVector);
    LeftHandCollisionComp->SetSphereRadius(10.f);
    LeftHandCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    LeftHandCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    LeftHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    LeftHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    LeftHandCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
    if (!PRPlayerState) return;
    AbilitySystemComponent = PRPlayerState->GetAbilitySystemComponent();
    AbilitySystemComponent->InitAbilityActorInfo(PRPlayerState, this);
    Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoInitialized();
    AttributeSet = PRPlayerState->GetAttributeSet();

    if (APRPlayerController* PC = GetController<APRPlayerController>())
    {
        if (ABaseHUD* HUD = PC->GetHUD<ABaseHUD>())
        {
            HUD->InitInGameHUD(PC, GetPlayerState(), AbilitySystemComponent, AttributeSet);
        }
    }
    InitializeDefaultAttributes();
    BindToTagChange();
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

void APRCharacter::SetSpeedMode(bool bSprintState)
{
    const float NewTargetSpeed = bSprintState ? NormalSpeed * SprintSpeedMultiplier : NormalSpeed;
    const float NewInterpRate = bSprintState ? SpeedInterpRateSprint : SpeedInterpRateWalk;

    // 클라 즉시 반영
    CurrentTargetSpeed = NewTargetSpeed;
    CurrentInterpRate = NewInterpRate;

    if (HasAuthority())
    {
        ReplicatedMaxWalkSpeed = NewTargetSpeed;
    }
}

void APRCharacter::SetCrouchSpeed()
{
    if (HasAuthority())
    {
        ReplicatedMaxWalkSpeed = CrouchSpeed;
        OnRep_MaxWalkSpeed();
    }
}

void APRCharacter::ResetToWalkSpeed()
{
    if (HasAuthority())
    {
        ReplicatedMaxWalkSpeed = NormalSpeed;
        CurrentTargetSpeed = NormalSpeed;
        OnRep_MaxWalkSpeed();
    }
}

void APRCharacter::OnRep_MaxWalkSpeed()
{
    CurrentTargetSpeed = ReplicatedMaxWalkSpeed;

    CurrentInterpRate =
        (ReplicatedMaxWalkSpeed > NormalSpeed)
        ? SpeedInterpRateSprint
        : SpeedInterpRateWalk;

    UE_LOG(LogTemp, Log, TEXT("OnRep_MaxWalkSpeed: MaxWalkSpeed = %f"), CurrentTargetSpeed);
}

void APRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (APRPlayerController* PlayerController = Cast<APRPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Started,
                    this,
                    &APRCharacter::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartSprint
                );
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopSprint
                );
            }

            if (PlayerController->CrouchAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->CrouchAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APRCharacter::StartCrouch
                );
                EnhancedInput->BindAction(
                    PlayerController->CrouchAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopCrouch
                );
            }

            if (PlayerController->AimAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->AimAction,
                    ETriggerEvent::Ongoing,
                    this,
                    &APRCharacter::StartAim
                );
                EnhancedInput->BindAction(
                    PlayerController->AimAction,
                    ETriggerEvent::Completed,
                    this,
                    &APRCharacter::StopAim
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

    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

    const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    FVector MoveDir = Forward * MoveInput.Y + Right * MoveInput.X;
    MoveDir.Normalize();

    float SpeedMultiplier = 1.0f;
    if (!GetCharacterMovement()->IsCrouching() && MoveInput.Y < 0.f)
    {
        SpeedMultiplier = BackwardSpeedMultiplier;
    }
    AddMovementInput(MoveDir, SpeedMultiplier);
}

void APRCharacter::StartAim(const FInputActionValue& Value)
{
    if (!HasAuthority())
        ServerSetAiming(true);
    bIsAiming = true;
    OnRep_IsAiming();
}

void APRCharacter::StopAim(const FInputActionValue& Value)
{
    if (!HasAuthority())
        ServerSetAiming(false);
    bIsAiming = false;
    OnRep_IsAiming();
}

void APRCharacter::StartJump(const FInputActionValue& value)
{
    if (GetCharacterMovement()->IsCrouching()) return;

    Jump();

    if (HasAuthority())
    {
        SetJustJumped(true);
    }
    else
    {
        ServerStartJump();
    }
}
void APRCharacter::ServerStartJump_Implementation()
{
    Jump(); // 서버도 점프 상태 반영
    SetJustJumped(true); // bJustJumped 리플리케이션으로 애니메이션 연동
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
    if (bIsAiming) return;

    SetSpeedMode(true);

    if (!HasAuthority())
    {
        ServerStartSprint();
    }

    bIsSprinting = true;
}

void APRCharacter::ServerStartSprint_Implementation()
{
    SetSpeedMode(true);
}

void APRCharacter::StopSprint(const FInputActionValue& value)
{
    SetSpeedMode(false); // 이걸로 대체

    if (!HasAuthority())
    {
        ServerStopSprint();
    }

    bIsSprinting = false; // 클라에서도 상태 유지
}

void APRCharacter::ServerStopSprint_Implementation()
{
    SetSpeedMode(false);
}

void APRCharacter::ServerRequestFootstep_Implementation(FVector Location, USoundBase* Sound)
{
    if (IsLocallyControlled()) return;

    UGameplayStatics::PlaySoundAtLocation(
        this,
        Sound,
        Location,
        FRotator::ZeroRotator,
        1.0f,
        1.0f,
        0.0f,
        FootstepAttenuation
    );
}

void APRCharacter::MulticastPlayFootstep_Implementation(FVector Location, USoundBase* Sound)
{
    float Volume = IsLocallyControlled() ? 0.3f : 1.0f;

    UGameplayStatics::PlaySoundAtLocation(
        this,
        Sound,
        Location,
        FRotator::ZeroRotator,
        Volume,
        1.0f,
        0.0f,
        FootstepAttenuation
    );
}

USoundBase* APRCharacter::GetFootstepSoundBySurface(EPhysicalSurface SurfaceType)
{
    if (FootstepSounds.Num() > 0)
    {
        USoundBase* SoundToPlay = FootstepSounds[FootstepSoundIndex];

        FootstepSoundIndex = (FootstepSoundIndex + 1) % FootstepSounds.Num(); // 순환

        return SoundToPlay;
    }

    return DefaultFootstepSound;
}

// 착지 시 발소리 재생
void APRCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    FVector Location = Hit.ImpactPoint;
    EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

    USoundBase* LandingSound = GetLandingSoundBySurface(SurfaceType);
    if (!LandingSound) return;

    // 멀티플레이 처리
    if (HasAuthority())
    {
        MulticastPlayLandingSound(Location, LandingSound);
    }
    else
    {
        ServerRequestLandingSound(Location, LandingSound);
    }
}

void APRCharacter::ServerRequestLandingSound_Implementation(FVector Location, USoundBase* Sound)
{
    MulticastPlayLandingSound(Location, Sound);
}

void APRCharacter::MulticastPlayLandingSound_Implementation(FVector Location, USoundBase* Sound)
{
    float Volume = IsLocallyControlled() ? 0.3f : 1.0f;

    UGameplayStatics::PlaySoundAtLocation(
        this,
        Sound,
        Location,
        FRotator::ZeroRotator,
        Volume,
        1.0f,
        0.0f,
        FootstepAttenuation
    );
}

USoundBase* APRCharacter::GetLandingSoundBySurface(EPhysicalSurface SurfaceType)
{
    return DefaultLandSound;
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
    DOREPLIFETIME(APRCharacter, bJustJumped);
    DOREPLIFETIME(APRCharacter, bIsAiming);
    DOREPLIFETIME(APRCharacter, ReplicatedMaxWalkSpeed);
    DOREPLIFETIME(APRCharacter, ReplicatedControlRotation);
    DOREPLIFETIME(APRCharacter, CurrentWeaponType);
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

void APRCharacter::OnRep_JustJumped()
{
}

void APRCharacter::OnRep_IsAiming()
{
    if (bIsAiming)
    {
        GetCharacterMovement()->bOrientRotationToMovement = false;
        bUseControllerRotationYaw = true;
    }
    else
    {
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
    }
}

void APRCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // 카메라 보간
    const float TargetArm = bIsAiming ? AimingArmLength : DefaultArmLength;
    const FVector TargetOffset = bIsAiming ? AimingSocketOffset : DefaultSocketOffset;
    SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetArm, DeltaSeconds, CameraInterpSpeed);
    SpringArmComp->SocketOffset = FMath::VInterpTo(SpringArmComp->SocketOffset, TargetOffset, DeltaSeconds, CameraInterpSpeed);

    // 이동 속도 보간
    float DesiredTargetSpeed = bIsAiming ? NormalSpeed * BackwardSpeedMultiplier : CurrentTargetSpeed;
    float MoveSpeed = 1.f;
    // MoveSpeed 어트리뷰트 반영
    if (UPRAttributeSet* AS = Cast<UPRAttributeSet>(GetAttributeSet()))
    {
        MoveSpeed = AS->GetMoveSpeed() / 100.f;
    }
    DesiredTargetSpeed *= MoveSpeed;

    float NewSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, DesiredTargetSpeed, DeltaSeconds, CurrentInterpRate);
    GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

    if (FMath::Abs(NewSpeed - DesiredTargetSpeed) < 0.5f)
    {
        GetCharacterMovement()->MaxWalkSpeed = DesiredTargetSpeed;
    }
    /*UE_LOG(LogTemp, Log, TEXT("CurrentTargetSpeed: %f, MaxWalkSpeed: %f"), CurrentTargetSpeed, GetCharacterMovement()->MaxWalkSpeed);*/

    // 점프 중일 땐 회전 막기 (에임 포함)
    if (bIsInAir)
    {
        if (bIsAiming)
        {
            // 에임 상태지만 점프 중이면 회전 금지
            bUseControllerRotationYaw = false;
        }
        GetCharacterMovement()->RotationRate = FRotator::ZeroRotator;
    }
    else
    {
        // 지상일 땐 에임 상태에 따라 회전 모드 설정
        if (bIsAiming)
        {
            bUseControllerRotationYaw = true;
            GetCharacterMovement()->bOrientRotationToMovement = false;
        }
        else
        {
            bUseControllerRotationYaw = false;
            GetCharacterMovement()->bOrientRotationToMovement = true;
        }

        GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
    }

    // 방향 계산
    const FVector Velocity = GetVelocity();
    const FRotator ActorRot = GetActorRotation();

    if (IsLocallyControlled())
    {
        MoveDirection = CalculateDirectionCustom(Velocity, ActorRot);
    }
    else if (HasAuthority())
    {
        float NewDir = CalculateDirectionCustom(Velocity, ActorRot);
        if (!FMath::IsNearlyEqual(MoveDirection, NewDir, 0.1f))
        {
            MoveDirection = NewDir;
        }
    }

    bIsInAir = GetCharacterMovement()->IsFalling();
    bIsCrouching = GetCharacterMovement()->IsCrouching();

    if (HasAuthority())
    {
        ReplicatedControlRotation = GetControlRotation(); //카메라 회전 리플리케이션
    }
    // 클라이언트에서 관전자용 회전 적용
    if (!IsLocallyControlled())
    {
        bool bIsBeingSpectated = !IsLocallyControlled() && ReplicatedControlRotation != FRotator::ZeroRotator;

        USpringArmComponent* SpringArm = FindComponentByClass<USpringArmComponent>();
        if (bIsBeingSpectated)
        {
            if (SpringArm)
            {
                // SpringArm이 Pawn 회전 안 따르게 설정 (한 번만 해도 됨)
                if (SpringArm->bUsePawnControlRotation)
                {
                    SpringArm->bUsePawnControlRotation = false;
                }

                SpringArm->SetWorldRotation(ReplicatedControlRotation);
            }
        }
    }
}

void APRCharacter::Die(/*const FHitResult& HitResult*/)
{
    Super::Die();

    if (HasAuthority())
    {
        GetAbilitySystemComponent()->ClearAllAbilities();
        if (APRPlayerState* PS = GetPlayerState<APRPlayerState>())
        {
            PS->SetIsDead(true);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState casting failed");
        }
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (APRPlayerController* PC = Cast<APRPlayerController>(It->Get()))
            {
                // 서버에서는 직접 호출
                PC->OnSpectateTargetDied(this);

                // 클라이언트에게도 전파
                PC->Client_OnSpectateTargetDied(this);
            }
        }

        StimuliSourceComponent->UnregisterFromPerceptionSystem();
    }
    //UnPossessed();
    MulticastRagdoll();
    // Ragdoll
    // MulticastRagdoll();
}

void APRCharacter::MulticastRagdoll_Implementation()
{
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        DisableInput(PC);
    }

    USkeletalMeshComponent* CharacterMesh = GetMesh();
    if (!CharacterMesh->IsRegistered())
    {
        UE_LOG(LogTemp, Warning, TEXT("Component not registered yet"));
        return;
    }
    CharacterMesh->SetSimulatePhysics(true);
    CharacterMesh->SetCollisionProfileName(FName("Ragdoll"));
    CharacterMesh->WakeAllRigidBodies();
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
    OnRep_JustJumped();

    if (bNewValue)
    {
        LastJumpDirection = MoveDirection;  // 점프 직전 방향 저장

        GetWorld()->GetTimerManager().SetTimer(
            JumpResetHandle,
            this,
            &APRCharacter::ResetJustJumped,
            0.2f,
            false
        );
    }
}

void APRCharacter::SetWeaponType(EWeaponType NewType)
{
    CurrentWeaponType = NewType;
    // 애니메이션 블루프린트에도 알릴 수 있도록 필요하면 이벤트 호출
}

void APRCharacter::ResetJustJumped()
{
    bJustJumped = false;
    OnRep_JustJumped();
}

void APRCharacter::ServerSetAiming_Implementation(bool bNewAiming)
{
    bIsAiming = bNewAiming;
    OnRep_IsAiming();
}

void APRCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent ||
        AbilitySystemComponent->HasMatchingGameplayTag(FPRGameplayTags::Get().Player_Block_InputPressed)) return;

    if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
    {
        ASC->AbilityInputTagPressed(InputTag);
    }
}

void APRCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent ||
        AbilitySystemComponent->HasMatchingGameplayTag(FPRGameplayTags::Get().Player_Block_InputReleased)) return;

    if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
    {
        ASC->AbilityInputTagReleased(InputTag);
    }
}

void APRCharacter::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent ||
        AbilitySystemComponent->HasMatchingGameplayTag(FPRGameplayTags::Get().Player_Block_InputHeld)) return;

    if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
    {
        ASC->AbilityInputTagHeld(InputTag);
    }
}