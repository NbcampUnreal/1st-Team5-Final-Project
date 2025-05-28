
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnRoarCollision.generated.h"


UCLASS()
class PAYROCK_API UAnimNotify_SpawnRoarCollision : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> RoarCollisionActorClass;
};