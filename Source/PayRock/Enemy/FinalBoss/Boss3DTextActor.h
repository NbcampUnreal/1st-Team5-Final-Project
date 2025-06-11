// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss3DTextActor.generated.h"

UCLASS()
class PAYROCK_API ABoss3DTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss3DTextActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	float CurrentTime = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Fade")
	 float FadeDuration = 2.0f; 
};
