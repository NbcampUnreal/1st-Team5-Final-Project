#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "LightSourceActor.generated.h"



UCLASS()
class PAYROCK_API ALightSourceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightSourceActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Light")
	TObjectPtr<UPointLightComponent> PointLight;
	

	UPROPERTY(EditAnywhere, Category = "Light")
	float DefaultIntensity = 5000.f;

	float StoredIntensity;

public:
	void TurnOffLight();
	void RestoreOriginalLight();
};
