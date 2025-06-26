// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Actor/BaseProjectile.h"
#include "BossWeaponFireProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PAYROCK_API ABossWeaponFireProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	ABossWeaponFireProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float InterpSpeed;

public:
	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

	
};
