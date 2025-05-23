// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "BlessingWidgetController.generated.h"

// class UBlessingComponent;

struct FBlessingData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveBlessingChange, const FBlessingData&, Blessing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPassiveBlessingChange, const FBlessingData&, Blessing);

UCLASS()
class PAYROCK_API UBlessingWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	// virtual void BindCallbacksToDependencies() override;

	void HandleBlessingSelection(const FBlessingData& Blessing);
	
private:
	// void InitializeBlessingComponentRef();

public:
	UPROPERTY(BlueprintAssignable, Category = "Blessing")
	FOnActiveBlessingChange OnActiveBlessingChange;
	UPROPERTY(BlueprintAssignable, Category = "Blessing")
	FOnPassiveBlessingChange OnPassiveBlessingChange;

	UPROPERTY()
	TArray<FBlessingData> BlessingsContainer;

	/*
private:
	UPROPERTY()
	UBlessingComponent* BlessingComponent;
	*/
};
