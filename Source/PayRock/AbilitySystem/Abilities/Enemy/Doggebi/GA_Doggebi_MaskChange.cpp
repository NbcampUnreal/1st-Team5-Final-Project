// PayRockGames


#include "GA_Doggebi_MaskChange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "PayRock/Enemy/MiddleBoss/Doggebi/DoggebiCharacter.h"

UGA_Doggebi_MaskChange::UGA_Doggebi_MaskChange()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Doggebi_MaskChange::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
			//랜덤 변수 넣어주기
		}
	}
}

void UGA_Doggebi_MaskChange::RandomMaskChange()
{
	Doggebi->RandomChangeMask();
}
