// PayRockGames


#include "AnimNotify_EndOverlap.h"

#include "PayRock/Enemy/EnemyCharacter.h"

void UAnimNotify_EndOverlap::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		Enemy->ToggleWeaponCollision(false); 
	}
}
