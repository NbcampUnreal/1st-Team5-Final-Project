// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CombatInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UGameplayEffect;
class UAttributeSet;

UCLASS()
class PAYROCK_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	USkeletalMeshComponent* GetWeapon() const { return Weapon.Get(); }

	/* GA - Hit React */
	void OnHitReactTagChanged(const FGameplayTag ChangedTag, int32 TagCount);
	UFUNCTION(BlueprintCallable)
	const UAnimMontage* GetHitReactMontage();
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReact = false;

	/* Death */
	UFUNCTION(BlueprintCallable)
	virtual void Die(/*const FHitResult& HitResult*/);
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetDeathMontage();
	UFUNCTION(BlueprintCallable)
	void ForceDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLootContainer();

	UFUNCTION(BlueprintCallable)
	void RecalculateSecondaryAttributes();
	void InternalRecalculateSecondaryAttributes();
	FTimerHandle StatRecalculateTimerHandle;
	bool bRecalculationScheduled = false;
	
	void InitializeDefaultAttributes();
	bool bAreAttributesInitialized = false;
	
protected:
	virtual void BeginPlay() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	
	virtual void InitAbilityActorInfo();
	virtual void AddCharacterAbilities();
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float& EffectLevel) const;
	void BindToTagChange();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName = FName("RightHandSocket");
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName = FName("LeftHandSocket");
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSocketName = FName("WeaponSocket");
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultPassiveAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributeEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitSecondaryAttributeEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitVitalAttributeEffect;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<UAnimMontage*> HitReactMontages;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<UAnimMontage*> DeathMontages;
};