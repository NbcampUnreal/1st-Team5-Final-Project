// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GA_BossAttackSelector.generated.h"


USTRUCT(BlueprintType)
struct FWeightedAttackTag
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FGameplayTag AttackTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Weight = 1.0f;
};


UCLASS()
class PAYROCK_API UGA_BossAttackSelector : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_BossAttackSelector();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TArray<FWeightedAttackTag> WeightedAttackTags;
	
	float GetBossHealthRatio() const;
};
