// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Character/Blessing/BlessingDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveDataSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlessingSaved, const TArray<FBlessingData>&, Blessings);

UCLASS()
class PAYROCK_API USaveDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/**
	 * Blessing
	 */
	UFUNCTION(BlueprintCallable, Category = "Blessings")
	const UBlessingDataAsset* GetBlessingDataAsset() const;
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	void SaveBlessing(const FBlessingData& Blessing);
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	const TArray<FBlessingData>& GetBlessingsContainer() const { return SavedBlessingsContainer; }
	
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	void SetEquippedActiveBlessing(const FBlessingData& Blessing) { EquippedActiveBlessing = Blessing; }
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	void SetEquippedPassiveBlessing(const FBlessingData& Blessing) { EquippedPassiveBlessing = Blessing; }
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	const FBlessingData& GetEquippedActiveBlessing() const { return EquippedActiveBlessing; }
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	const FBlessingData& GetEquippedPassiveBlessing() const { return EquippedPassiveBlessing; }

	/**
	 * Money
	 */
	UFUNCTION(BlueprintCallable, Category = "Money")
	void SetSavedMoney(const int32& Amount) { SavedMoney = Amount; }
	UFUNCTION(BlueprintCallable, Category = "Money")
	const int32& GetSavedMoney() const { return SavedMoney; }

	FOnBlessingSaved OnBlessingSaved;

private:
	UPROPERTY()
	TArray<FBlessingData> SavedBlessingsContainer;
	UPROPERTY()
	FBlessingData EquippedActiveBlessing;
	UPROPERTY()
	FBlessingData EquippedPassiveBlessing;
	UPROPERTY()
	UBlessingDataAsset* BlessingDataAsset;

	UPROPERTY()
	int32 SavedMoney;
};
