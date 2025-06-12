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
	GetCharacterMovement()->MaxWalkSpeed = 350.f;

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

	bReplicates = true;
	SetReplicateMovement(true);
}

void ANecroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ANecroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void ANecroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void ANecroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// MoveSpeed 어트리뷰트 반영
	float MoveSpeed = 1.f;
	if (UPRAttributeSet* AS = Cast<UPRAttributeSet>(GetAttributeSet()))
	{
		MoveSpeed = AS->GetMoveSpeed() / 100.f;
	}
	float NewSpeed = BaseWalkSpeed * MoveSpeed;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

// Called to bind functionality to input
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

