// PayRockGames

#include "PRCharacter.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Blessing/BlessingComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Input/PRInputComponent.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/Player/PRPlayerController.h"
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
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = false;

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

    NormalSpeed = 300.0f;
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
    if (!AbilitySystemComponent) return;
    
    AbilitySystemComponent->InitAbilityActorInfo(PRPlayerState, this);
    Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoInitialized();
    AttributeSet = PRPlayerState->GetAttributeSet();

    PRAttributeSet = Cast<UPRAttributeSet>(AttributeSet);
    
    InitializeDefaultAttributes();
    BindToTagChange();
}

void APRCharacter::BindToTagChange()
{
    Super::BindToTagChange();

    // Invisible Tag Binding
    AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Status_Buff_Invisible).AddUObject(
        BlessingComponent, &UBlessingComponent::OnInvisibleTagChanged);
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

void APRCharacter::StartJump(const FInputActionValue& Value)
{
    if (GetCharacterMovement()->IsCrouching()) return;
    if (!AbilitySystemComponent || !GE_JumpManaCost || !PRAttributeSet) return;
    if (PRAttributeSet->GetMana() < 10.f) return; // 마나 부족 시 점프 금지

    Jump(); // 클라이언트에서 즉시 점프 반응

    // 로컬에서 실행하지 않고 서버에 요청
    ServerStartJump();
    
    if (CanDoubleJump())
    {
        Server_DoubleJump();
    }
    else
    {
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
}

void APRCharacter::ServerStartJump_Implementation()
{
    if (!AbilitySystemComponent || !GE_JumpManaCost || !PRAttributeSet) return;
    if (PRAttributeSet->GetMana() < 10.f) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Not enough Mana to jump!"));
        return;
    }

    FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(GE_JumpManaCost, 1.f, Context);

    if (Spec.IsValid())
    {
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data);
        Jump(); // 점프 수행
        SetJustJumped(true); // 애님 연동용 리플리케이션
    }
}

void APRCharacter::StopJump(const FInputActionValue& value)
{
    StopJumping();
}

/*** Double Jump ***/
void APRCharacter::Server_DoubleJump_Implementation()
{
    bIsDoubleJumping = true;
    
    FVector LaunchVelocity = GetActorForwardVector();\
    LaunchVelocity *= 1000.f;
    LaunchVelocity.Z = DoubleJumpZAmount;
    LaunchCharacter(LaunchVelocity, false, true);
    
    Multicast_DoubleJumpMontage(true);
}

void APRCharacter::Server_DoubleJumpLanded_Implementation()
{
    bIsDoubleJumping = false;
    
    Multicast_DoubleJumpMontage(false);
}

void APRCharacter::Multicast_DoubleJumpMontage_Implementation(bool bIsJump)
{
    UAnimMontage* MontageToPlay = bIsJump ? DoubleJumpMontage : DoubleJumpLandedMontage;
    if (!MontageToPlay) return;
    
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->StopAllMontages(0.1f);
        float MontageLength = AnimInstance->Montage_Play(MontageToPlay);

        if (MontageLength > 0.f && !bIsJump)
        {
            DisableInput(GetLocalViewingPlayerController());
            
            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(
                TimerHandle,
                [this]()
                {
                    EnableInput(GetLocalViewingPlayerController());
                },
                MontageLength,
                false
            );
        }
    }
}

bool APRCharacter::CanDoubleJump()
{
    return bIsInAir && !bIsDoubleJumping &&
        GetAbilitySystemComponent()->HasMatchingGameplayTag(FPRGameplayTags::Get().Status_Buff_DoubleJump);
}
/*** Double Jump ***/

/*** Spin ***/
void APRCharacter::StartSpin()
{
    bShouldSpin = true;
}

void APRCharacter::StopSpin()
{
    bShouldSpin = false;
}
/*** Spin ***/

void APRCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X * MouseSensitivity);
    AddControllerPitchInput(LookInput.Y * MouseSensitivity);
}

void APRCharacter::StartSprint(const FInputActionValue& Value)
{
    if (bIsAiming || bIsSprinting || !AbilitySystemComponent || !GE_SprintManaCost || !PRAttributeSet) return;

    // 마나가 5 미만이면 스프린트 불가
    if (PRAttributeSet->GetMana() < 5.f) return;

    SetSpeedMode(true);

    if (!HasAuthority())
    {
        ServerStartSprint();
    }

    bIsSprinting = true;
}

void APRCharacter::ServerStartSprint_Implementation()
{
    if (bIsAiming || bIsSprinting || !AbilitySystemComponent || !GE_SprintManaCost || !PRAttributeSet) return;

    if (PRAttributeSet->GetMana() < 5.f) return;

    SetSpeedMode(true);
    bIsSprinting = true;

    if (!ActiveSprintGEHandle.IsValid())
    {
        FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE_SprintManaCost, 1.f, ContextHandle);
        if (SpecHandle.IsValid())
        {
            ActiveSprintGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
        }
    }
}

void APRCharacter::StopSprint(const FInputActionValue& Value)
{
    if (!bIsSprinting) return;

    SetSpeedMode(false);

    if (!HasAuthority())
    {
        ServerStopSprint();
    }

    bIsSprinting = false;
}

void APRCharacter::ServerStopSprint_Implementation()
{
    SetSpeedMode(false);
    bIsSprinting = false;

    if (ActiveSprintGEHandle.IsValid())
    {
        AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveSprintGEHandle);
        ActiveSprintGEHandle.Invalidate();
    }
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

    if (bIsDoubleJumping)
    {
        Server_DoubleJumpLanded();    
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

void APRCharacter::ServerPlayAttackSound_Implementation(USoundBase* Sound, FVector Location)
{
    MulticastPlayAttackSound(Sound, Location);
}

void APRCharacter::MulticastPlayAttackSound_Implementation(USoundBase* Sound, FVector Location)
{
    if (HasAuthority() && IsLocallyControlled())
    {
        // 서버장이자 로컬 컨트롤러면, 이미 재생했으니 멀티캐스트에서 재생하지 않음
        return;
    }

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
    
    if (bHit && Hit.GetActor())
    {
        if (Hit.GetActor()->Implements<UPRInterface>())
        {
            return Hit.GetActor();
        }
    }

    return nullptr;
}

void APRCharacter::Interact(const FInputActionValue& value)
{
    AActor* Target = FindInteractableActor();
    if (!Target)
    {
        return;
    }
    
    if (IPRInterface* Interactable = Cast<IPRInterface>(Target))
    {
        if (Interactable->CanInteract(this))
        {
            Interactable->Interact(this);
        }
    }
}

void APRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    /* Gameplay Status */
    DOREPLIFETIME(APRCharacter, bIsDead);
    DOREPLIFETIME(APRCharacter, bIsExtracted);
    DOREPLIFETIME(APRCharacter, bIsInvisible);

    /* Movement Status */
    DOREPLIFETIME(APRCharacter, MoveDirection);
    DOREPLIFETIME(APRCharacter, bIsSprinting);
    DOREPLIFETIME(APRCharacter, bIsCrouching);
    DOREPLIFETIME(APRCharacter, bIsInAir);
    DOREPLIFETIME(APRCharacter, bIsGuarding);
    DOREPLIFETIME(APRCharacter, bJustJumped);
    DOREPLIFETIME(APRCharacter, bIsDoubleJumping)
    DOREPLIFETIME(APRCharacter, bIsAiming);
    DOREPLIFETIME(APRCharacter, bShouldSpin);
    DOREPLIFETIME(APRCharacter, ReplicatedMaxWalkSpeed);
    DOREPLIFETIME(APRCharacter, ReplicatedControlRotation);
    DOREPLIFETIME(APRCharacter, CurrentWeaponType);

    /* Combo Status */
    DOREPLIFETIME(APRCharacter, bResetCombo);
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

    /* SPIN - early return */
    if (bShouldSpin)
    {
        AddActorLocalRotation(FRotator(0.f, SpinSpeed * DeltaSeconds, 0.f));
        return;
    }
    
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
    // 디버그용 마나 확인
    if (IsLocallyControlled() && PRAttributeSet)
    {
        float Mana = PRAttributeSet->GetMana();
        if (IsSprinting() && Mana < 5.f)
        {
            if (HasAuthority())
            {
                StopSprint(FInputActionValue());
            }
            else
            {
                ServerStopSprint();
            }
        }

        // GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("Current Mana: %.1f"), Mana));
    }
}

void APRCharacter::Die(FVector HitDirection)
{
    Super::Die(HitDirection);

    bIsDead = true;
    StimuliSourceComponent->UnregisterFromPerceptionSystem();
    
    if (HasAuthority())
    {
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
    }
}

void APRCharacter::OnExtraction()
{
    bIsExtracted = true;
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StimuliSourceComponent->UnregisterFromPerceptionSystem();
    DisableInput(GetLocalViewingPlayerController());
    
    if (HasAuthority())
    {
        GetAbilitySystemComponent()->ClearAllAbilities();
		
        // Remove ALL active gameplay effects
        FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer());
        GetAbilitySystemComponent()->RemoveActiveEffects(Query);
        
        MulticastExtraction();
    }
    
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();
}

void APRCharacter::MulticastExtraction_Implementation()
{
    // NOT WORKING - play extraction montage?
    Crouch();
    
    // Spawn Niagara
    FVector SpawnLoc = GetActorLocation();
    SpawnLoc.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    if (!ExtractionNiagara)
    {
        UE_LOG(LogTemp, Warning, TEXT("Extraction Niagara system not set")); return;
    }
    UNiagaraComponent* SpawnedExtractionNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(), ExtractionNiagara, SpawnLoc);

    if (GetWorld() && !GetWorld()->bIsTearingDown)
    {
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(
            TimerHandle,
            this,
            &APRCharacter::HideCharacter,
            HideDelay
            );
    }
}

void APRCharacter::HideCharacter()
{
    if (IsValid(this))
    {
        SetActorHiddenInGame(true);
    }
}

void APRCharacter::StartComboTimer()
{
    bResetCombo = false;
    GetWorldTimerManager().SetTimer(
        ComboTimerHandle,
        this,
        &APRCharacter::SetResetCombo,
        ComboTime
    );
}

void APRCharacter::SetResetCombo()
{
    bResetCombo = true;
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
    // Client_ForceUpdateWeaponType(NewType);
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