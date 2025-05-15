// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StartOverlap.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UAnimNotify_StartOverlap : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
