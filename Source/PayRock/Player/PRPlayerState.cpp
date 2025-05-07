// PayRockGames

#include "PRPlayerState.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

APRPlayerState::APRPlayerState()
{
	bReplicates = true;
	
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UPRAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPRAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* APRPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
