// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PRAnimNotify_PlayFootstep.generated.h"

UCLASS()
class PAYROCK_API UPRAnimNotify_PlayFootstep : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
