// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "CycloneActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/GA_Boss_Magic_Base.h"
#include "GA_Cyclone.generated.h"


UCLASS()
class PAYROCK_API UGA_Cyclone : public UGA_Boss_Magic_Base
{
	GENERATED_BODY()

public:
	UGA_Cyclone();

protected:
	virtual void OnAuraEffectComplete() override;

private:
	UPROPERTY(EditAnywhere, Category = "Cyclone")
	TSubclassOf<ACycloneActor> CycloneClass;

	UPROPERTY(EditAnywhere, Category = "Cyclone")
	float ForwardOffset = 300.f;

	UPROPERTY(EditAnywhere, Category = "Cyclone")
	float CycloneDuration = 6.f;

	TWeakObjectPtr<ACycloneActor> SpawnedCyclone;

	FTimerHandle DestroyTimerHandle;

	void DestroyCyclone();
	void OnCycloneEnded();
};