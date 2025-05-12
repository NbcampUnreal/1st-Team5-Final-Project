// PayRockGames

#include "BaseProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PayRock/Actor/BaseProjectile.h"
#include "PayRock/Character/CombatInterface.h"


void UBaseProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bSpawned = false;
}

void UBaseProjectileSpell::SpawnProjectile(const FGameplayTag& SocketTag)
{
	if (!GetAvatarActorFromActorInfo() || bSpawned) return;
	
	FTransform SpawnTransform;
	const FVector SocketLocation =
		ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorForwardVector().ToOrientationQuat());

	ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->SourceAbility = this;

	bSpawned = true;
	Projectile->FinishSpawning(SpawnTransform);
}
