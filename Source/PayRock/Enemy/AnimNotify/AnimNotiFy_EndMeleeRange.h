// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotiFy_EndMeleeRange.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UAnimNotiFy_EndMeleeRange : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};