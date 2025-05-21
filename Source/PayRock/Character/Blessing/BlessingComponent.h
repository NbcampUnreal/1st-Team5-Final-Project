// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "BlessingComponent.generated.h"

class UAbilitySystemComponent;
class UBlessingDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPassiveBlessingChanged, UBlessingDataAsset*, DataAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveBlessingChanged, UBlessingDataAsset*, DataAsset);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PAYROCK_API UBlessingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBlessingComponent();
	
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_EquipActiveBlessing(UBlessingDataAsset* BlessingDataAsset);
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_EquipPassiveBlessing(UBlessingDataAsset* BlessingDataAsset);
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_UnequipActiveBlessing();
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_UnequipPassiveBlessing();
	
	UFUNCTION(Client, Reliable, Category = "Blessing")
	void Client_BroadcastActiveBlessing(UBlessingDataAsset* BlessingDataAsset);
	UFUNCTION(Client, Reliable, Category = "Blessing")
	void Client_BroadcastPassiveBlessing(UBlessingDataAsset* BlessingDataAsset);

	UBlessingDataAsset* GetEquippedPassiveBlessingData() const { return EquippedPassiveBlessingData; }
	UBlessingDataAsset* GetEquippedActiveBlessingData() const { return EquippedActiveBlessingData; }

	FOnActiveBlessingChanged OnActiveBlessingChange;
	FOnPassiveBlessingChanged OnPassiveBlessingChange;

private:
	UAbilitySystemComponent* GetAbilitySystemComponent();

private:
	UPROPERTY()
	UAbilitySystemComponent* CachedAbilitySystemComponent;
	UPROPERTY()
	UBlessingDataAsset* EquippedPassiveBlessingData;
	UPROPERTY()
	UBlessingDataAsset* EquippedActiveBlessingData;

	FActiveGameplayEffectHandle PassiveBlessingHandle;
	FGameplayAbilitySpecHandle ActiveBlessingHandle;
};
