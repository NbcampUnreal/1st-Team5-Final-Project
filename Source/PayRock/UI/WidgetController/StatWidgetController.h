// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "GameplayEffectTypes.h"
#include "StatWidgetController.generated.h"

class UBlessingComponent;
class UBlessingDataAsset;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChange, FString, AttributeName, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveBlessingChange, UBlessingDataAsset*, DataAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPassiveBlessingChange, UBlessingDataAsset*, DataAsset);

UCLASS()
class PAYROCK_API UStatWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnAttributeChange OnAttributeChangeDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Blessing")
	FOnActiveBlessingChange OnActiveBlessingChange;
	UPROPERTY(BlueprintAssignable, Category = "Blessing")
	FOnPassiveBlessingChange OnPassiveBlessingChange;

private:
	void InitializeAttributesArray();
	void InitializeBlessingComponentRef();

	void BroadcastAttributeChange(const FOnAttributeChangeData& Data);
	UFUNCTION()
	void BroadcastActiveBlessingChange(UBlessingDataAsset* BlessingDataAsset);
	UFUNCTION()
	void BroadcastPassiveBlessingChange(UBlessingDataAsset* BlessingDataAsset);
	
	UPROPERTY()
	TArray<FGameplayAttribute> Attributes;

	UPROPERTY()
	UBlessingComponent* BlessingComponent;
};
