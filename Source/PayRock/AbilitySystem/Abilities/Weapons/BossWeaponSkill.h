// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseMontageDamageSkill.h"
#include "BossWeaponSkill.generated.h"

UENUM(BlueprintType)
enum class EBossWeaponSkillType : uint8
{
	Fire,
	Lightning,
	Spike,
	Roar,
	MAX
};

UCLASS()
class PAYROCK_API UBossWeaponSkill : public UBaseMontageDamageSkill
{
	GENERATED_BODY()

public:
	
	virtual void OnEventReceived(FGameplayEventData Payload) override;

protected:
	void ExecuteFireSkill();
	void ExecuteRoarSkill();
	void ExecuteSkill(TSubclassOf<UGameplayEffect> EffectToTarget, TSubclassOf<UGameplayEffect> EffectToSelf);
	
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon")
	float Radius = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Fire")
	TSubclassOf<class ABossWeaponFireProjectile> FireProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Lightning")
	TSubclassOf<UGameplayEffect> LightningTargetEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Lightning")
	TSubclassOf<UGameplayEffect> LightningSelfEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Spike")
	TSubclassOf<UGameplayEffect> SpikeTargetEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Spike")
	TSubclassOf<UGameplayEffect> SpikeSelfEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Water")
	TSubclassOf<UGameplayEffect> WaterTargetEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Water")
	TSubclassOf<UGameplayEffect> WaterSelfEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Roar")
	TSubclassOf<UGameplayEffect> RoarSelfEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "BossWeapon|Roar")
	float LaunchSpeed = 600.f;
};
