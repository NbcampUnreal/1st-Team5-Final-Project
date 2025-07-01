#include "AnimNotify_ThrowingActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UAnimNotify_ThrowingActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !ThrowingActorClass) return;

	ACharacter* OwnerChar = Cast<ACharacter>(MeshComp->GetOwner());
	if (!OwnerChar) return;
	
	const FVector SpawnLocation = MeshComp->GetSocketLocation(SpawnSocketName);
	const FRotator SpawnRotation = OwnerChar->GetActorRotation();
	FVector LaunchDir = OwnerChar->GetActorForwardVector();
	LaunchDir = LaunchDir.GetSafeNormal();
	
	FActorSpawnParameters Params;
	Params.Owner = OwnerChar;
	Params.Instigator = OwnerChar;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AThrowingWeaponBase* Weapon = MeshComp->GetWorld()->SpawnActor<AThrowingWeaponBase>(ThrowingActorClass, SpawnLocation, SpawnRotation, Params);
	if (Weapon)
	{
		if (Weapon->ProjectileMovement)
		{
			Weapon->ProjectileMovement->Velocity = LaunchDir * TravelSpeed;
		}
	}
}
