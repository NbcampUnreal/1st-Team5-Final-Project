// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PayRock/Actor/Enemy/ThrowingWeapon/ThrowingWeaponBase.h"
#include "AnimNotify_ThrowingActor.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UAnimNotify_ThrowingActor : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Throwing")
	TSubclassOf<AThrowingWeaponBase> ThrowingActorClass;
	
	UPROPERTY(EditAnywhere, Category = "Throwing")
	FName SpawnSocketName = "LeftHandSocket";


	UPROPERTY(EditAnywhere, Category = "Throwing")
	float TravelSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = "Throwing")
	float VerticalLiftRatio = 0.25f;
};