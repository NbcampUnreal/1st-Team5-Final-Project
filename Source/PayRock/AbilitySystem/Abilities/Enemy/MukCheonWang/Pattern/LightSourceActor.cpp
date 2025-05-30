#include "LightSourceActor.h"
#include "Components/PointLightComponent.h"

ALightSourceActor::ALightSourceActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	SetRootComponent(PointLight);
	PointLight->SetIntensity(DefaultIntensity);
	PointLight->SetVisibility(true);
}

void ALightSourceActor::BeginPlay()
{
	Super::BeginPlay();
	
	StoredIntensity = PointLight->Intensity;
}

void ALightSourceActor::TurnOffLight()
{
	if (PointLight)
	{
		StoredIntensity = PointLight->Intensity;
		PointLight->SetIntensity(0.f);
	}
}

void ALightSourceActor::RestoreOriginalLight()
{
	if (PointLight)
	{
		PointLight->SetIntensity(StoredIntensity);
	}
}
