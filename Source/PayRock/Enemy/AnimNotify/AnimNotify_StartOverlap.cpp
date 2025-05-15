// PayRockGames


#include "AnimNotify_StartOverlap.h"

#include "PayRock/Enemy/EnemyCharacter.h"


void UAnimNotify_StartOverlap::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		Enemy->ToggleWeaponCollision(true); 
	}
}
