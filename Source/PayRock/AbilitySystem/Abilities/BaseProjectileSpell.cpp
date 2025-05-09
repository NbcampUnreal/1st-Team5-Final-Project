// PayRockGames

#include "BaseProjectileSpell.h"
#include "PayRock/Actor/BaseProjectile.h"

void UBaseProjectileSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&ActivationInfo)) return;
	
	FTransform SpawnTransform;
	//TODO: maybe set location to the location of the top socket in the weapon? - use ICombatInterface
	SpawnTransform.SetLocation(GetAvatarActorFromActorInfo()->GetActorLocation());
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
