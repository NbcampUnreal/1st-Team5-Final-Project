// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "OverlayWidgetController.h"
#include "NecroWidgetController.generated.h"

UCLASS()
class PAYROCK_API UNecroWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	virtual void HandleRemoval() override;
	
	UPROPERTY()
	FOnNotificationRequestSignature OnNotificationRequest;
	
	UPROPERTY()
	TMap<FGameplayTag, FOnCooldownChanged> CooldownDelegates;

protected:
	UFUNCTION()
	void BroadcastExtractionEnabled();
	
	void CooldownChanged(const FGameplayTag Tag, int32 TagCount);
	void BroadcastCooldown(const FGameplayTag& Tag);

private:
	UPROPERTY()
	TMap<FGameplayTag, FTimerHandle> CooldownUpdateTimers;
};
