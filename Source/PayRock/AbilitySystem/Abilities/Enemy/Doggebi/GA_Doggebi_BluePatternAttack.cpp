// PayRockGames


#include "GA_Doggebi_BluePatternAttack.h"

UGA_Doggebi_BluePatternAttack::UGA_Doggebi_BluePatternAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Doggebi_BluePatternAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Doggebi_BluePatternAttack::BindCallbackToWeaponCollision()
{
}

void UGA_Doggebi_BluePatternAttack::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void UGA_Doggebi_BluePatternAttack::ToggleColiision(bool isActive)
{
}

void UGA_Doggebi_BluePatternAttack::ResetDamaegdActors()
{
}

void UGA_Doggebi_BluePatternAttack::EndAttackBlackboardState()
{
}
