// PayRockGames


#include "Boss3DTextActor.h"

#include "Components/PrimitiveComponent.h"

// Sets default values
ABoss3DTextActor::ABoss3DTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoss3DTextActor::BeginPlay()
{
	Super::BeginPlay();
	
	UPrimitiveComponent* Prim = FindComponentByClass<UPrimitiveComponent>();
	if (!Prim) return;

	int32 NumMaterials = Prim->GetNumMaterials();
	for (int32 i = 0; i < NumMaterials; ++i)
	{
		UMaterialInterface* BaseMat = Prim->GetMaterial(i);
		if (!BaseMat) continue;

		UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMat, this);
		if (DynMat)
		{
			DynMat->SetScalarParameterValue(TEXT("Opacity"), 1.0f);
			DynamicMaterials.Add(DynMat);
			Prim->SetMaterial(i, DynMat);
		}
	}
}

// Called every frame
void ABoss3DTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;
	float Alpha = FMath::Clamp(1.0f - (CurrentTime / FadeDuration), 0.0f, 1.0f);

	for (UMaterialInstanceDynamic* Mat : DynamicMaterials)
	{
		if (Mat)
		{
			Mat->SetScalarParameterValue(TEXT("Opacity"), Alpha);
		}
	}

	if (CurrentTime >= FadeDuration)
	{
		Destroy();
	}
}

