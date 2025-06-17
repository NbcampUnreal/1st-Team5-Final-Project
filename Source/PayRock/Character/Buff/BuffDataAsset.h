// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BuffDataAsset.generated.h"

UCLASS()
class PAYROCK_API UBuffDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION()
	UAnimMontage* GetReactMontageByTag(const FGameplayTag& Tag) const;
	
	UPROPERTY(EditAnywhere, Category = "Debuff|Montages")
	TMap<FGameplayTag, UAnimMontage*> ReactMontages;

};
