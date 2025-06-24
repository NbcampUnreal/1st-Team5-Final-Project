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

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	PlayerCharacter,
	DefaultMonster,
	JangSanTiger,
	Kappa,
	MarketClown,
	OneEyed,
	Samurai,
	SamuraiStatue,
	Skeleton,
	Doggebi,
	Boss
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Type")
	ECharacterType CharacterType = ECharacterType::PlayerCharacter;

	//�ǰݻ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Sound")
	USoundBase* HitReactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Sound")
	USoundBase* HitVoiceSound;

	// ĳ���� Ÿ�� �Ǻ��� ��ƿ
	UFUNCTION(BlueprintCallable, Category = "Combat|Type")
	bool IsPlayerCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Type")
	bool IsMonster() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Type")
	bool IsBoss() const;

	/* Death */
	UFUNCTION(BlueprintCallable)
	virtual void Die(FVector HitDirection = FVector::ZeroVector);
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRagdoll(const FVector& HitDirection);
	UFUNCTION(BlueprintCallable)
	void ForceDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLootContainer();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetDeathMontage();
	
	void InitializeDefaultAttributes();
	bool bAreAttributesInitialized = false;
	bool bOnlyRelevantToOwner = true;
	
protected:
	virtual void BeginPlay() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	
	virtual void InitAbilityActorInfo();
	virtual void AddCharacterAbilities();
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float& EffectLevel) const;
	virtual void BindToTagChange();

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> RecalculateSecondaryEffect;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<UAnimMontage*> HitReactMontages;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<UAnimMontage*> DeathMontages;
};