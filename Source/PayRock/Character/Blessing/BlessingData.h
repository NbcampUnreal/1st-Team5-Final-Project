// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "BlessingData.generated.h"

UENUM(BlueprintType)
enum class EBlessingType : uint8
{
	Passive,
	Active
};

UENUM(BlueprintType)
enum class EBlessingRarity : uint8
{
	DONOTUSE,
	Normal,
	Rare,
	Unique,
	Epic
};

USTRUCT(BlueprintType)
struct FBlessingData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	EBlessingType BlessingType = EBlessingType::Active;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	EBlessingRarity BlessingRarity = EBlessingRarity::Normal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	FText BlessingName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	FText KoreanName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true), Category = "Blessing")
	FText BlessingDescription = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true), Category = "Blessing")
	FText PenaltyDescription = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayAbility> ActiveAbilityClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayEffect> PassiveEffectClass = nullptr;

	// Penalty effect for active blessings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayEffect> PenaltyEffect = nullptr;

	FBlessingData() {}
};