// PayRockGames

#include "PRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/Player/PRPlayerController.h"
#include "PayRock/UI/HUD/BaseHUD.h"

APRCharacter::APRCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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
