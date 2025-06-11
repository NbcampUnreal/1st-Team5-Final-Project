// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "FireballProjectile.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/GA_Boss_Magic_Base.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "GA_VFX_Fireball.generated.h"


UCLASS()
class PAYROCK_API UGA_VFX_Fireball : public UGA_Boss_Magic_Base
{
	GENERATED_BODY()

public:
	UGA_VFX_Fireball();

protected:
	virtual void OnAuraEffectComplete() override;

	void SpawnNextFireball();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	TSubclassOf<AFireballProjectile> FireballClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	float MinForwardOffset = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	float MaxForwardOffset = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	float SideOffsetRange = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	float VerticalOffset = 100.f;

	FTimerHandle FireballSequenceTimerHandle;
	
	TArray<TWeakObjectPtr<AActor>> DetectedTargets;
	int32 TotalFireballsToSpawn = 0;
	int32 CurrentFireballIndex = 0;
};