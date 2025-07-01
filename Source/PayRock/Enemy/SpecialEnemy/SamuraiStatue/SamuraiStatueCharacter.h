// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "SamuraiStatueCharacter.generated.h"


class UGA_EnemyNormalAttack;
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

	
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void ToggleColiision(bool isActive);

	bool bHit = false;
	
virtual void Die(FVector HitDiretion) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	UCapsuleComponent* WeaponCollision;

	UPROPERTY()
	UGA_EnemyNormalAttack* CurrentSamuraiAttackGA = nullptr;
};
