// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_Boss_Magic_Base.generated.h"

UCLASS()
class PAYROCK_API UGA_Boss_Magic_Base : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Boss_Magic_Base();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	UPROPERTY()
	TWeakObjectPtr<AActor> AvatarActor;

	UPROPERTY()
	TWeakObjectPtr<class AMukCheonWangCharacter> Caster;

	UPROPERTY()
	FTimerHandle AuraDelayTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "VFX|Aura")
	float AuraDelayTime = 1.0f;

	FGameplayAbilitySpecHandle CurrentSpecHandle;
	FGameplayAbilityActorInfo* CurrentActorInfo;
	FGameplayAbilityActivationInfo CurrentActivationInfo;
	
	virtual void OnAuraEffectComplete();
	
	void FinishAbility();
};