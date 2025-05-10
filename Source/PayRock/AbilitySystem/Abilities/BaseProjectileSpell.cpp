// PayRockGames

#include "BaseProjectileSpell.h"
#include "PayRock/Actor/BaseProjectile.h"
#include "PayRock/Character/CombatInterface.h"


void UBaseProjectileSpell::SpawnProjectile(const FGameplayTag& SocketTag)
{
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->HasAuthority()) return;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(
		ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag));
	SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorForwardVector().ToOrientationQuat());

	ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//TODO: Give the Projectile a Gameplay Effect Spec for causing Damage

	Projectile->FinishSpawning(SpawnTransform);
}
