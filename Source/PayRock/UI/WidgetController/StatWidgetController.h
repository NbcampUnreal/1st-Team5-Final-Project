// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "StatWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChange, const FString&, AttributeName, const float&, NewValue);

UCLASS()
class PAYROCK_API UStatWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	void InitializeAttributesArray();

private:
	void BroadcastAttributeChange(const FOnAttributeChangeData& Data);

public:
	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnAttributeChange OnAttributeChangeDelegate;
	
	UPROPERTY()
	TArray<FGameplayAttribute> Attributes;
};
