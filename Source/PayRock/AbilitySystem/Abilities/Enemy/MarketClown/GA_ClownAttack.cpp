#include "GA_ClownAttack.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"

UGA_ClownAttack::UGA_ClownAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ClownAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AMarketClownMonster* Boss = Cast<AMarketClownMonster>(ActorInfo->AvatarActor.Get());
	if (!Boss)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			if (BB->GetValueAsBool(FName("bIsAttacking")))
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				return;
			}
		}
	}
	
	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC)
	{
		ResetBlackboardAttackState(Boss);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	TSubclassOf<UGameplayAbility> SelectedAbility = nullptr;

	switch (Boss->CurrentMask)
	{
	case ETalMaskType::Yangban:
		SelectedAbility = WeaponAbility_Yangban;
		break;
	case ETalMaskType::Baekjeong:
		SelectedAbility = WeaponAbility_Baekjeong;
		break;
	default:
		break;
	}

	if (!SelectedAbility)
	{
		ResetBlackboardAttackState(Boss);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const bool bSuccess = ASC->TryActivateAbilityByClass(SelectedAbility);
	ResetBlackboardAttackState(Boss);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, !bSuccess);
}

void UGA_ClownAttack::ResetBlackboardAttackState(AMarketClownMonster* Boss)
{
	if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsBusy"), false);
		}
	}
}
