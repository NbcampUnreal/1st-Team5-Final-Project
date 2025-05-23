// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BlessingData.h"
#include "Engine/DataAsset.h"
#include "BlessingDataAsset.generated.h"

UCLASS(BlueprintType)
class PAYROCK_API UBlessingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing|Passive")
	TMap<FString, FBlessingData> PassiveBlessings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing|Active")
	TMap<FString, FBlessingData> ActiveBlessings;

	UFUNCTION(BlueprintCallable, Category = "Blessings")
	FBlessingData GetNormalBlessingByNameAndType(const FText& Name, const EBlessingType& Type);

	UFUNCTION(BlueprintCallable, Category = "Blessings")
	void SetBlessingRarity(FBlessingData& Blessing, EBlessingRarity Rarity);
};
