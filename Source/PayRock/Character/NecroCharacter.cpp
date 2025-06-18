// PayRockGames

#include "NecroCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Input/PRInputComponent.h"
#include "PayRock/Player/PRPlayerController.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/UI/Manager/UIManager.h"

ANecroCharacter::ANecroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

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
	GetMesh()->SetIsReplicated(true);

	bReplicates = true;
	SetReplicateMovement(true);
}

void ANecroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Block);
	/* TODO: ECR_Block to the Salt / Garlic item so the character can be hit */
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

/*
 *	Input Binding & Movement
 */
void ANecroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
					&ANecroCharacter::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ANecroCharacter::Look
				);
			}
		}
	}

	// Ability Input binding
	if (UPRInputComponent* PRInputComponent = Cast<UPRInputComponent>(PlayerInputComponent))
	{
		PRInputComponent->BindAbilityActions(InputConfig, this,
			&ANecroCharacter::AbilityInputTagPressed,
			&ANecroCharacter::AbilityInputTagReleased,
			&ANecroCharacter::AbilityInputTagHeld);
	}
}

void ANecroCharacter::Destroyed()
{
	if (HasAuthority() && IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->ClearAllAbilities();
		
		// Remove ALL active gameplay effects
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer());
		AbilitySystemComponent->RemoveActiveEffects(Query);
	}
	
	Super::Destroyed();
}

void ANecroCharacter::Move(const FInputActionValue& Value)
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
	
	AddMovementInput(MoveDir);
}

void ANecroCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

/*
 *	Ability System
 */
void ANecroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	InitializeMeshVisibility();

	if (IsLocallyControlled())
	{
		InitializeOverlayMaterial();
	}
}

void ANecroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();

	InitializeMeshVisibility();
	
	if (IsLocallyControlled())
	{
		InitializeOverlayMaterial();
	}
}

void ANecroCharacter::InitializeMeshVisibility()
{
	if (IsLocallyControlled())
	{
		GetMesh()->SetHiddenInGame(false);
	}
	else
	{
		GetMesh()->SetHiddenInGame(true);
	}
}

void ANecroCharacter::InitializeOverlayMaterial()
{
	UMaterialInterface* OverlayMaterial = GetMesh()->GetOverlayMaterial();
	if (!OverlayMaterialDynamic && OverlayMaterial)
	{
		OverlayMaterialDynamic = UMaterialInstanceDynamic::Create(OverlayMaterial, this);
		GetMesh()->SetOverlayMaterial(OverlayMaterialDynamic);
	}
}

void ANecroCharacter::InitAbilityActorInfo()
{
	APRPlayerState* PRPlayerState = GetPlayerState<APRPlayerState>();
	if (!PRPlayerState) return;
	
	AbilitySystemComponent = PRPlayerState->GetAbilitySystemComponent();
	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->InitAbilityActorInfo(PRPlayerState, this);
	Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoInitialized();

	AttributeSet = PRPlayerState->GetAttributeSet();

	InitializeDefaultAttributes();
	AddCharacterAbilities();
	BindToTagChange();

	if (IsLocallyControlled())
	{
		if (APRPlayerController* PlayerController = Cast<APRPlayerController>(GetController()))
		{
			PlayerController->RemoveDeathOptions();
		}
		UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
		if (!UIManager) return;
		UIManager->RemoveAllWidgets();
		UIManager->RemoveAllWidgetControllers();
		// TODO: Show Necro HUD
		UIManager->ShowWidget(EWidgetCategory::InGameHUD);
	}
}

void ANecroCharacter::AddCharacterAbilities()
{
	if (!HasAuthority()) return;
	if (UPRAbilitySystemComponent* PR_ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
	{
		PR_ASC->AddCharacterAbilities(DefaultAbilities);
		PR_ASC->AddCharacterPassiveAbilities(DefaultPassiveAbilities);
	}
}

void ANecroCharacter::BindToTagChange()
{
	if (IsValid(AbilitySystemComponent))
	{
		FGameplayTag Tag = FPRGameplayTags::Get().Status_Necro_Visible;
		AbilitySystemComponent->RegisterGameplayTagEvent(Tag).AddUObject(
		this, &ANecroCharacter::OnVisibleTagChanged);

		OnVisibleTagChanged(Tag, 0);
	}
}

void ANecroCharacter::OnVisibleTagChanged(const FGameplayTag Tag, int32 TagCount)
{
	bool bMakeVisible = TagCount > 0;

	if (IsLocallyControlled() && OverlayMaterialDynamic)
	{
		OverlayMaterialDynamic->SetScalarParameterValue(TEXT("Exponent"),
			bMakeVisible ? 0.0f : 3.0f);
	}
	
	if (!IsLocallyControlled())
	{
		GetMesh()->SetHiddenInGame(!bMakeVisible);
	}
}

void ANecroCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!AbilitySystemComponent ||
		AbilitySystemComponent->HasMatchingGameplayTag(FPRGameplayTags::Get().Player_Block_InputPressed)) return;

	if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void ANecroCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!AbilitySystemComponent ||
		AbilitySystemComponent->HasMatchingGameplayTag(FPRGameplayTags::Get().Player_Block_InputReleased)) return;

	if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void ANecroCharacter::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!AbilitySystemComponent ||
		AbilitySystemComponent->HasMatchingGameplayTag(FPRGameplayTags::Get().Player_Block_InputHeld)) return;

	if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->AbilityInputTagHeld(InputTag);
	}
}

