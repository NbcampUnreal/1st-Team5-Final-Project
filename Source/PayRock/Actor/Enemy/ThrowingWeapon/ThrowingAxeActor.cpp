// PayRockGames

#include "ThrowingAxeActor.h"


AThrowingAxeActor::AThrowingAxeActor()
{
	PrimaryActorTick.bCanEverTick = true;
}



void AThrowingAxeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (WeaponMesh)
	{
		FRotator NewRotation = WeaponMesh->GetRelativeRotation() + RotationSpeed * DeltaTime;
		WeaponMesh->SetRelativeRotation(NewRotation);
	}
}

