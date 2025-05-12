// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
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
	
};
