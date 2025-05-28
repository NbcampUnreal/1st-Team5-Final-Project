// PayRockGames


#include "AnimNotify_SpawnRoarCollision.h"


void UAnimNotify_SpawnRoarCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner || !RoarCollisionActorClass) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	FActorSpawnParameters Params;
	Params.Owner = Owner;
	
	FVector SpawnLoc = Owner->GetActorLocation();
	FRotator SpawnRot = Owner->GetActorRotation();

	World->SpawnActor<AActor>(RoarCollisionActorClass, SpawnLoc, SpawnRot, Params);
}
