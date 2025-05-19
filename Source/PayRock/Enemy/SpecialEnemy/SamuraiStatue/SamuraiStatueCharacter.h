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

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	UCapsuleComponent* WeaponCollision;
};
