// AnimNotify_LaunchToTarget.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_LaunchToTarget.generated.h"

UCLASS()
class PAYROCK_API UAnimNotify_LaunchToTarget : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Launch")
	float HorizontalSpeed = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Launch")
	float VerticalBoost = 300.f;
};
