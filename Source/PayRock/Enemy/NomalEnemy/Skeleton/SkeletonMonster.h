// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
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

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<USphereComponent> WeaponCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName CollisionSocketName = FName("CollisionSocket");
};
