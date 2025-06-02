// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "BaseWeaponAbility.generated.h"

class APRCharacter;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NormalAttack = 0,
	DashAttack,
	JumpAttack,
	/*CrouchAttack*/
};

UCLASS()
class PAYROCK_API UBaseWeaponAbility : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void ToggleCollision(bool bShouldEnable);
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void GetCollisionComponents(USkeletalMeshComponent* WeaponMesh, const FName& SocketName);
	void BindCallbackToCollision();
	void UpdateCurrentAttackType(APRCharacter* PlayerCharacter);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CollisionSocketName;
	
protected:
	UPROPERTY()
	TArray<UShapeComponent*> CollisionComponents;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboTimeLimit = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> EndingMontageArray;

	UPROPERTY(BlueprintReadOnly)
	EAttackType CurrentAttackType = EAttackType::NormalAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DashAttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> JumpAttackMontage;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TObjectPtr<UAnimMontage> CrouchAttackMontage;

	bool bHit = false;
};
