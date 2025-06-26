// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BuffDataAsset.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

struct FGameplayTag;
class ABaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PAYROCK_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuffComponent();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	UFUNCTION()
	void OnBlindTagChange(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION()
	void OnKnockbackTagChange(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION()
	void OnFrozenTagChange(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION()
	void OnShockedTagChange(const FGameplayTag Tag, int32 TagCount);

	bool IsKnockedBack() const { return bIsKnockedBack; }
	void StartKnockbackRecovery();
	// === Multicast Functions ===
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyFrozen();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RemoveFrozen();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyShocked(const FGameplayTag Tag);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RemoveShocked();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Client, Unreliable)
	void Client_BroadcastTagChange(const FGameplayTag& Tag, int32 TagCount);
	
	float PlayMontageByTag(const FGameplayTag Tag);
	void KnockbackRecovery();
	void DisableMovement();
	void EnableMovement();
	void CancelActiveAbilities();
	
	UPROPERTY()
	ABaseCharacter* OwningPRCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff|Knockback")
	float KnockbackForce;
	UPROPERTY(EditDefaultsOnly, Category = "Debuff|Knockback")
	float KnockbackVertical;

	bool bIsKnockedBack;
	FTimerHandle KnockbackRecoveryTimer;
};
