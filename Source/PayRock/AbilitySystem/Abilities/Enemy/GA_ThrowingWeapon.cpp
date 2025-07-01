#include "GA_ThrowingWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UGA_ThrowingWeapon::UGA_ThrowingWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGA_ThrowingWeapon::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Caster = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Caster || !ThrowingWeaponClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector SpawnLocation = Caster->GetMesh()->GetSocketLocation(SocketName);
	const FRotator SpawnRotation = Caster->GetActorRotation();
	const FVector LaunchDir = (Caster->GetActorForwardVector() + FVector(0.f, 0.f, VerticalLiftRatio)).GetSafeNormal();

	FActorSpawnParameters Params;
	Params.Owner = Caster;
	Params.Instigator = Caster;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AThrowingWeaponBase* Weapon = GetWorld()->SpawnActor<AThrowingWeaponBase>(ThrowingWeaponClass, SpawnLocation, SpawnRotation, Params);
	if (Weapon)
	{
		
		Weapon->InstigatorAbility = this;
		Weapon->Damage = Damage;
		Weapon->DamageTypeTag = DamageTypeTag;
		Weapon->DamageEffectClass = DamageEffectClass;

		
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Caster);
		Weapon->SourceASC = ASC;

		Weapon->LaunchWeapon(LaunchDir * TravelSpeed);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}