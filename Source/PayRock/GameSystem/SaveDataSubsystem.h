// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveDataSubsystem.generated.h"

class UBlessingDataAsset;

UCLASS()
class PAYROCK_API USaveDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	void SaveBlessing(const TArray<UBlessingDataAsset*>& BlessingArray);
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	const TArray<UBlessingDataAsset*>& GetBlessingContainer() { return SavedBlessingsContainer; }
	
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	void SetEquippedActiveBlessing(UBlessingDataAsset* Blessing) { EquippedActiveBlessing = Blessing; }
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	void SetEquippedPassiveBlessing(UBlessingDataAsset* Blessing) { EquippedPassiveBlessing = Blessing; }
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	UBlessingDataAsset* GetEquippedActiveBlessing() const { return EquippedActiveBlessing; }
	UFUNCTION(BlueprintCallable, Category = "Blessing")
	UBlessingDataAsset* GetEquippedPassiveBlessing() const { return EquippedPassiveBlessing; }
	
	UFUNCTION(BlueprintCallable, Category = "Money")
	void SetSavedMoney(const int32& Amount) { SavedMoney = Amount; }
	UFUNCTION(BlueprintCallable, Category = "Money")
	int32 GetSavedMoney() const { return SavedMoney; }

private:
	UPROPERTY()
	TArray<UBlessingDataAsset*> SavedBlessingsContainer;
	UPROPERTY()
	UBlessingDataAsset* EquippedActiveBlessing;
	UPROPERTY()
	UBlessingDataAsset* EquippedPassiveBlessing;

	UPROPERTY()
	int32 SavedMoney;
};
