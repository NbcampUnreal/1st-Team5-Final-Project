// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PayRock/UI/WidgetController/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationRequestSignature, ENotificationType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExtractionSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveBlessingChangedSignature, UTexture2D*, IconTexture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAccessorySkillChangedSignature, UTexture2D*, IconTexture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSkillChangedSignature, UTexture2D*, IconTexture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChanged, float, RemainingTime);

UCLASS(BlueprintType, Blueprintable)
class PAYROCK_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	virtual void HandleRemoval() override;

	UPROPERTY()
	FOnHealthChangedSignature OnHealthChanged;
	UPROPERTY()
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	UPROPERTY()
	FOnManaChangedSignature OnManaChanged;
	UPROPERTY()
	FOnMaxManaChangedSignature OnMaxManaChanged;

	UPROPERTY()
	FOnNotificationRequestSignature OnNotificationRequest;
	UPROPERTY()
	FOnDeathSignature OnDeath;
	UPROPERTY()
	FOnExtractionSignature OnExtraction;

	UPROPERTY()
	FOnActiveBlessingChangedSignature OnActiveBlessingChanged;
	UPROPERTY()
	FOnAccessorySkillChangedSignature OnAccessorySkillChanged;
	UPROPERTY()
	FOnWeaponSkillChangedSignature OnWeaponSkillChanged;
	UPROPERTY()
	TMap<FGameplayTag, FOnCooldownChanged> CooldownDelegates;

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;

	UFUNCTION()
	void BroadcastExtractionEnabled();
	void BroadcastDeath() const;
	void BroadcastExtraction() const;
	
	void BroadcastAccessorySkillChange(const FName& AccessoryID) const;
	void BroadcastWeaponSkillChange(const FName& WeaponID) const;
	
	void CooldownChanged(const FGameplayTag Tag, int32 TagCount);
	void BroadcastCooldown(const FGameplayTag& Tag);

private:
	UPROPERTY()
	TMap<FGameplayTag, FTimerHandle> CooldownUpdateTimers;
};

