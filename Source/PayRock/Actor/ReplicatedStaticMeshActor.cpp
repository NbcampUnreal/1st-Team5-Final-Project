#include "ReplicatedStaticMeshActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"

AReplicatedStaticMeshActor::AReplicatedStaticMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	MeshComponent->SetIsReplicated(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComponent->SetMobility(EComponentMobility::Static);
}

void AReplicatedStaticMeshActor::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComponent)
	{
		if (GetNetMode() == NM_DedicatedServer || HasAuthority())
		{
			if (ServerMaterial)
			{
				MeshComponent->SetMaterial(0, ServerMaterial);
			}
		}
		else if (GetNetMode() == NM_Client)
		{
			if (ClientMaterial)
			{
				MeshComponent->SetMaterial(0, ClientMaterial);
			}
		}
	}
}

void AReplicatedStaticMeshActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
