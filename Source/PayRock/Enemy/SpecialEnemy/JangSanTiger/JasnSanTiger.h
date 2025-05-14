// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "JasnSanTiger.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API AJasnSanTiger : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AJasnSanTiger();

	void BeginTelegraph();
	void UpdateTelegraph(float DeltaTime);
	void EndTelegraph();

	UPROPERTY(VisibleAnywhere, Category = "Telegraph")
	UDecalComponent* TelegraphDecal;

	UPROPERTY(EditAnywhere, Category = "Telegraph")
	UMaterialInstanceDynamic* TelegraphMat;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Telegraph")
	float maxLength=10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charge")
	UBoxComponent* ChargeCollision;
protected:
	virtual void Tick(float DeltaTime) override;

private:
	bool bTelegraphing = false;
	float CurrentLength = 0.f;
	
	
};
