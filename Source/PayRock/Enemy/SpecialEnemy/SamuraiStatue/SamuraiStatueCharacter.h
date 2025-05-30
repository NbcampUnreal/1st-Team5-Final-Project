// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "SamuraiStatueCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API ASamuraiStatueCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	ASamuraiStatueCharacter();

	void PauseAnimation();
	void ResumeAnimation();

	UFUNCTION(BlueprintCallable)
	void SetDie(bool isdie);

	UFUNCTION(BlueprintCallable)
	bool GetDie();
	
virtual void Die(FVector HitDiretion) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	UCapsuleComponent* WeaponCollision;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Dead State")
	bool bisDead = false;
};
