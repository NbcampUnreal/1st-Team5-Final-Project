#include "GA_BossMeleeAttack_Base.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_BossMeleeAttack_Base::UGA_BossMeleeAttack_Base()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_BossMeleeAttack_Base::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bHit = false;
	BossCharacter = Cast<AMukCheonWangCharacter>(ActorInfo->AvatarActor.Get());
	if (!BossCharacter) return;

	BossCharacter->CurrentAbilityRef = this;

	if (AAIController* AICon = Cast<AAIController>(BossCharacter->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", true);
			AICon->StopMovement();
		}
	}
}

void UGA_BossMeleeAttack_Base::EndAttackBlackboardState()
{
	if (!BossCharacter) return;

	BossCharacter->ClearHitActors(); 

	BossCharacter->CurrentAbilityRef = nullptr;

	AController* Controller = Cast<AController>(GetActorInfo().OwnerActor->GetInstigatorController());
	if (!Controller) return;

	UBlackboardComponent* BB = Controller->FindComponentByClass<UBlackboardComponent>();
	if (BB)
	{
		BB->SetValueAsBool(FName("bIsNearPlayer"), false);
		BB->SetValueAsBool("bIsAttacking", false);
	}
}