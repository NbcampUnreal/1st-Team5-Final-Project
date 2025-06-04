// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Character/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PAYROCK_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	void ToggleWeaponCollision(bool bEnable);


	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomAttackMontage() const;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomDetectMontage() const;
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetBattleState() const { return bIsBattle; }
	void SetBattleState(bool Value){ bIsBattle = Value; };

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void AddCharacterAbilities() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (AllowPrivateAccess = "true"))
	int32 SplitLevel = 0;
	bool bIsClone = false;

	//Sound
	UFUNCTION(BlueprintCallable,NetMulticast, Unreliable)
	void Multicast_PlayAttackSound(USoundBase* Sound);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundAttenuation* AttackAttenuation;
	
protected:
	virtual void Die(FVector HitDirection = FVector::ZeroVector) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	bool bIsBattle = false;
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<AActor> ContainerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> DetectMontages;


	
};
