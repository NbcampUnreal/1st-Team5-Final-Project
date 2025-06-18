// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatedStaticMeshActor.generated.h"

UCLASS()
class PAYROCK_API AReplicatedStaticMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AReplicatedStaticMeshActor();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Material")
	TObjectPtr<UMaterialInterface> ServerMaterial;

	UPROPERTY(EditAnywhere, Category = "Material")
	TObjectPtr<UMaterialInterface> ClientMaterial;
};