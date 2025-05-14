// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "SkeletonMonster.generated.h"

UCLASS()
class PAYROCK_API ASkeletonMonster : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	ASkeletonMonster();


protected:
	virtual void BeginPlay() override;
};
