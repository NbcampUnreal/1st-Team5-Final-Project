// PayRockGames


#include "GA_EnemyNormalAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "PayRock/Enemy/SpecialEnemy/SamuraiStatue/SamuraiStatueCharacter.h"
#include "GameFramework/Character.h"

UGA_EnemyNormalAttack::UGA_EnemyNormalAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_EnemyNormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bHit = false;
	ACharacter* Avatar = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	
	samurai = Cast<ASamuraiStatueCharacter>(ActorInfo->AvatarActor.Get());
	if (!Avatar) return;
	
	if (samurai)
	{
		samurai->CurrentSamuraiAttackGA = this;
		samurai->bHit = false;
	}
	
	if (AAIController* AICon = Cast<AAIController>(Avatar->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", true);
			AICon->StopMovement();
		}
	}
	
	
}


void UGA_EnemyNormalAttack::EndAttackBlackboardState()
{
	if (!samurai) return;

	samurai->ToggleColiision(false);
	samurai->CurrentSamuraiAttackGA = nullptr;

}
