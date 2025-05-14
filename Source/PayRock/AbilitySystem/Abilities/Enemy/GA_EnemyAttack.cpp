// PayRockGames

#include "GA_EnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UGA_EnemyAttack::UGA_EnemyAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_EnemyAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo) return;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	float TimeRemaining = 0.f;
	float Duration = 0.f;
	GetCooldownTimeRemainingAndDuration(Handle, ActorInfo, TimeRemaining, Duration);

	if (TimeRemaining > 0.f)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(ActorInfo->AvatarActor))
	{
		if (Enemy->GetAttackMontage())
		{
			Enemy->PlayAnimMontage(Enemy->GetAttackMontage());
		}
		if (AAIController* AICon = Cast<AAIController>(Enemy->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bIsAttacking", true);
			}
		}
	}

	if (CooldownGameplayEffect)
	{
		ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
