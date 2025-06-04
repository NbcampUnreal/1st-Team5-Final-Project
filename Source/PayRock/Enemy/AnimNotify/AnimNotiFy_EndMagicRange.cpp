// PayRockGames


#include "AnimNotiFy_EndMagicRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

void UAnimNotiFy_EndMagicRange::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		if (AAIController* AICon = Cast<AAIController>(Enemy->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bInMagicRange", false);
			}
		}
	}
}
