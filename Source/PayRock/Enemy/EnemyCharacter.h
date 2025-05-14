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

	UAnimMontage* GetAttackMontage() const {return AttackMontage;}

	UFUNCTION()
	void ToggleWeaponCollision(bool bEnable);
	
	UAnimMontage* GetDetectMontage() const {return DetectMontage;}
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void AddCharacterAbilities() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DetectMontage;
};
