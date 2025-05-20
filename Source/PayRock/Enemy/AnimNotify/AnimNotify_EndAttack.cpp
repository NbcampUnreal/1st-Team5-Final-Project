// PayRockGames


#include "AnimNotify_EndAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

void UAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		if (AAIController* AICon = Cast<AAIController>(Enemy->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bIsAttacking", false);
				BB->SetValueAsBool("bIsBusy", false);
			}
		}
	}
}
