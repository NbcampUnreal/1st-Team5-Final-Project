#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss3DTextActor.generated.h"

UCLASS()
class PAYROCK_API ABoss3DTextActor : public AActor
{
	GENERATED_BODY()

public:
	ABoss3DTextActor();

protected:
	virtual void BeginPlay() override;
	void UpdateEffect();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float ElapsedTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float FadeDuration = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float UpdateInterval = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float MoveSpeed = 30.f;
	
private:
	FVector InitialScale;
	FTimerHandle EffectTimerHandle;

	FVector TargetScale = FVector(1.5f);
};
