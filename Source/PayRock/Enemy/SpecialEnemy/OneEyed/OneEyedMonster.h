// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "OneEyedMonster.generated.h"

UCLASS()
class PAYROCK_API AOneEyedMonster : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AOneEyedMonster();

protected:
	virtual void BeginPlay() override;
	
};
