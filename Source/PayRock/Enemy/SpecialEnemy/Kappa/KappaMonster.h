// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "KappaMonster.generated.h"

UCLASS()
class PAYROCK_API AKappaMonster : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AKappaMonster();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void OnPlayerDetected(AActor* DetectedActor);
	
};