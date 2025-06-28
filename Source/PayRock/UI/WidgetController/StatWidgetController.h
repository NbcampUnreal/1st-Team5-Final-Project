// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.h"
#include "StatWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChange, const FString&, AttributeName, const float&, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveBlessingInfoSet, const FBlessingData&, BlessingData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSkillChange, const FSkillData&, SkillData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAccessorySkillChange, const FSkillData&, SkillData);

UCLASS()
class PAYROCK_API UStatWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

private:
	void InitializeAttributesArray();
	void BroadcastAttributeChange(const FOnAttributeChangeData& Data) const;
	void BroadcastWeaponSkillChange(const FName& WeaponID) const;
	void BroadcastAccessorySkillChange(const FName& AccessoryID) const;

public:
	UPROPERTY()
	FOnAttributeChange OnAttributeChangeDelegate;
	UPROPERTY()
	FOnActiveBlessingInfoSet OnActiveBlessingInfoSet;
	UPROPERTY()
	FOnAccessorySkillChange OnAccessorySkillChanged;
	UPROPERTY()
	FOnWeaponSkillChange OnWeaponSkillChanged;
	
	UPROPERTY()
	TArray<FGameplayAttribute> Attributes;
	UPROPERTY()
	TArray<FString> AttributeNames;
};
