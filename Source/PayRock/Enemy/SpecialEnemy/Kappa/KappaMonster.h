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
	virtual void Tick(float DeltaTime) override;


	
public:
	void OnPlayerDetected(AActor* DetectedActor);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	USphereComponent* HeadCollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName HeadSocketName = TEXT("HeadSocket");
};