// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "BlessingData.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "BlessingComponent.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPassiveBlessingChanged, const FBlessingData&, DataAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveBlessingChanged, const FBlessingData&, DataAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlessingAcquired, const FBlessingData&, DataAsset);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PAYROCK_API UBlessingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBlessingComponent();
	
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_EquipActiveBlessing(const FBlessingData& Blessing);
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_EquipPassiveBlessing(const FBlessingData& Blessing);
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_UnequipActiveBlessing();
	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_UnequipPassiveBlessing();

	UFUNCTION(Server, Reliable, Category = "Blessing")
	void Server_AddBlessing(const FBlessingData& Blessing);
	
	UFUNCTION(Client, Reliable, Category = "Blessing")
	void Client_BroadcastActiveBlessing(const FBlessingData& Blessing);
	UFUNCTION(Client, Reliable, Category = "Blessing")
	void Client_BroadcastPassiveBlessing(const FBlessingData& Blessing);
	UFUNCTION(Client, Reliable, Category = "Blessing")
	void Client_BroadcastBlessingAcquired(const FBlessingData& Blessing);

	const FBlessingData& GetEquippedPassiveBlessingData() const { return EquippedPassiveBlessingData; }
	const FBlessingData& GetEquippedActiveBlessingData() const { return EquippedActiveBlessingData; }

	FOnActiveBlessingChanged OnActiveBlessingChange;
	FOnPassiveBlessingChanged OnPassiveBlessingChange;
	FOnBlessingAcquired OnBlessingAcquired;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
private:
	UFUNCTION()
	void OnRep_AcquiredBlessings();
	
	UAbilitySystemComponent* GetAbilitySystemComponent();

private:
	UPROPERTY()
	UAbilitySystemComponent* CachedAbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_AcquiredBlessings, Category = "Blessing")
	TArray<FBlessingData> AcquiredBlessings;
	UPROPERTY()
	FBlessingData EquippedPassiveBlessingData;
	UPROPERTY()
	FBlessingData EquippedActiveBlessingData;

	FActiveGameplayEffectHandle PassiveBlessingHandle;
	FGameplayAbilitySpecHandle ActiveBlessingHandle;

	/**
	 * TEST
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	FBlessingData TestActiveBlessing;
	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	FBlessingData TestPassiveBlessing;
};
