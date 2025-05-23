// PayRockGames


#include "GA_Doggebi_HawlingAbility.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "PayRock/Enemy/MiddleBoss/Doggebi/DoggebiCharacter.h"


UGA_Doggebi_HawlingAbility::UGA_Doggebi_HawlingAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Doggebi_HawlingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	ACharacter* Avatar = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	
	Doggebi = Cast<ADoggebiCharacter>(ActorInfo->AvatarActor.Get());
	if (!Doggebi) return;

	if (AAIController* AICon = Cast<AAIController>(Doggebi->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			AICon->StopMovement();
		}
	}
}
