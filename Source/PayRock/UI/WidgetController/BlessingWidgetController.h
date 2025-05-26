// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "BlessingWidgetController.generated.h"

class UBlessingComponent;
class UBlessingDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveBlessingChange, UBlessingDataAsset*, DataAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPassiveBlessingChange, UBlessingDataAsset*, DataAsset);

UCLASS()
class PAYROCK_API UBlessingWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
private:
	void InitializeBlessingComponentRef();
	void LoadBlessingsFromSubsystem();

	UFUNCTION()
	void BroadcastActiveBlessingChange(UBlessingDataAsset* BlessingDataAsset);
	UFUNCTION()
	void BroadcastPassiveBlessingChange(UBlessingDataAsset* BlessingDataAsset);

public:
	UPROPERTY(BlueprintAssignable, Category = "Blessing")
	FOnActiveBlessingChange OnActiveBlessingChange;
	UPROPERTY(BlueprintAssignable, Category = "Blessing")
	FOnPassiveBlessingChange OnPassiveBlessingChange;

	UPROPERTY()
	TArray<UBlessingDataAsset*> BlessingsContainer;

private:
	UPROPERTY()
	UBlessingComponent* BlessingComponent;
};
