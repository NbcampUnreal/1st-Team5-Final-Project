// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EndOverlap.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UAnimNotify_EndOverlap : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
