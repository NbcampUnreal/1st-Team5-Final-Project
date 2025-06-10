// PayRockGames

#include "PRPlayerState.h"

#include "PRPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/PRCharacter.h"

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
			if (bIsDead) PC->Client_ShowDeathOptions();
		}
	}
}

void APRPlayerState::SetIsExtracted(bool bExtracted)
{
	if (HasAuthority())
	{
		bIsExtracted = bExtracted;
		if (APRPlayerController* PC = Cast<APRPlayerController>(GetOwningController()))
		{
			if (bIsExtracted) PC->Client_ShowDeathOptions();
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

void APRPlayerState::ForceDeath()
{
	if (APRCharacter* Character = Cast<APRCharacter>(GetPawn()))
	{
		Character->ForceDeath();
	}
}

void APRPlayerState::Extract()
{
    if (HasAuthority())
    {
        SetIsExtracted(true);
    }
	if (APRCharacter* Character = Cast<APRCharacter>(GetPawn()))
	{
		Character->OnExtraction();
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
