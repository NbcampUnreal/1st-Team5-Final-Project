// PayRockGames

#include "PRPlayerState.h"

#include "PRPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/PRGameplayTags.h"
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

void APRPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APRPlayerState, Level);
	DOREPLIFETIME(APRPlayerState, bIsDead);
	DOREPLIFETIME(APRPlayerState, bIsExtracted);
}

UAbilitySystemComponent* APRPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APRPlayerState::SetIsDead(bool bDead)
{
	if (HasAuthority())
	{
		bIsDead = bDead;
		if (APRPlayerController* PC = Cast<APRPlayerController>(GetOwningController()))
		{
			PC->Client_ShowDeathOptions();
		}
	}
}

void APRPlayerState::SetIsExtracted(bool bExtracted)
{
	if (HasAuthority())
	{
		bIsExtracted = bExtracted;	
	}
}

void APRPlayerState::ForceDeath()
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FPRGameplayTags::Get().Status_Life_Dead);
	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
}

void APRPlayerState::Extract()
{
    if (HasAuthority())
    {
        SetIsExtracted(true);
    }
}

void APRPlayerState::OnRep_Level(int32 OldLevel)
{
	if (Level != OldLevel)
	{
		OnLevelChangeDelegate.Broadcast(Level);
	}
}

void APRPlayerState::OnRep_bIsDead(bool Old_bIsDead)
{
	if (bIsDead && !Old_bIsDead)
	{
		OnDeathDelegate.Broadcast();
	}
}

void APRPlayerState::OnRep_bIsExtracted()
{
	if (bIsExtracted)
	{
		OnExtractionDelegate.Broadcast();
		
	}
}
