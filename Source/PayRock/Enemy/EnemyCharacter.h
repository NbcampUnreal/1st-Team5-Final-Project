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


	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomAttackMontage() const;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomDetectMontage() const;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetBattleState() const { return bIsBattle; }
	void SetBattleState(bool Value){ bIsBattle = Value; };

	void ToggleWeaponCollision(bool bEnable);
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void AddCharacterAbilities() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	bool bIsBattle = false;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> DetectMontages;
	
};
