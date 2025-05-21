// PayRockGames


#include "GA_Doggebi_RedPatternAttack.h"

UGA_Doggebi_RedPatternAttack::UGA_Doggebi_RedPatternAttack()
{
}

void UGA_Doggebi_RedPatternAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Doggebi_RedPatternAttack::BindCallbackToWeaponCollision()
{
}

void UGA_Doggebi_RedPatternAttack::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void UGA_Doggebi_RedPatternAttack::ToggleColiision(bool isActive)
{
}

void UGA_Doggebi_RedPatternAttack::ResetDamaegdActors()
{
}

void UGA_Doggebi_RedPatternAttack::EndAttackBlackboardState()
{
}
