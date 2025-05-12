// PayRockGames

#include "BaseProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PayRock/PRGameplayTags.h"
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

	// Damage Gameplay Effect
	const UAbilitySystemComponent* SourceASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle EffectSpecHandle =
		SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext()); 
	FPRGameplayTags GameplayTags = FPRGameplayTags::Get();
	float Magnitude = 50.f; // TODO: Calculation based on attributes
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Damage, Magnitude);
	Projectile->DamageEffectSpecHandle = EffectSpecHandle;
	
	Projectile->FinishSpawning(SpawnTransform);
}
