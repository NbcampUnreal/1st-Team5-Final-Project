// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayGradualSound.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UAnimNotify_PlayGradualSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_PlayGradualSound();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> Sound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float MaxHearingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float BaseVolume;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float LoudnessForAI;
};