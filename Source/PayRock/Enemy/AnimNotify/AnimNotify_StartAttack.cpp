// PayRockGames


#include "AnimNotify_StartAttack.h"

#include "AnimNotify_StartAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyCharacter.h"

void UAnimNotify_StartAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		if (AAIController* AICon = Cast<AAIController>(Enemy->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bIsAttacking", true);
			}
		}
	}
}
