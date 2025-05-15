// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StartAttack.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UAnimNotify_StartAttack : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};


