// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "BlessingDataAsset.generated.h"

UENUM(BlueprintType)
enum class EBlessingType : uint8
{
	Passive,
	Active
};

UENUM(BlueprintType)
enum class EBlessingRarity : uint8
{
	None = 0,
	Normal = 1,
	Rare = 2,
	Unique = 3,
	Epic = 4
};

UCLASS(BlueprintType)
class PAYROCK_API UBlessingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	EBlessingType BlessingType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	EBlessingRarity BlessingRarity;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	FText BlessingName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	FText BuffDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	FText PenaltyDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayAbility> ActiveAbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayEffect> PassiveEffectClass;

	// Penalty effect for active blessings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayEffect> PenaltyEffect;
};
