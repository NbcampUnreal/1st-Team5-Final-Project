// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
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

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionComponent")
	USphereComponent* LeftHandCollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionComponent")
	USphereComponent* RightHandCollisionComp;
	
};