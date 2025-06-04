// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotiFy_EndMagicRange.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UAnimNotiFy_EndMagicRange : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};