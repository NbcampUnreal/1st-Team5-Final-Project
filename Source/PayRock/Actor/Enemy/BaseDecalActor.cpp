#include "BaseDecalActor.h"
#include "Components/DecalComponent.h"

ABaseDecalActor::ABaseDecalActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("AuraDecal"));
	SetRootComponent(Decal);
	Decal->DecalSize = FVector(64.f, 64.f, 64.f);
	Decal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); 
}

void ABaseDecalActor::Init(AActor* Target)
{
	if (!Target) return;

	FollowTarget = Target;
	InitialLocation = Target->GetActorLocation();

	Direction = Target->GetActorForwardVector(); 
	SetActorLocation(InitialLocation);
	SetActorRotation(Direction.Rotation());
	SetActorScale3D(FVector(0.1f)); 
}

void ABaseDecalActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseDecalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;

	
	FVector Offset = Direction * OffsetSpeed * DeltaTime;
	AddActorWorldOffset(Offset);

	
	float Scale = FMath::Min(MaxScale, GetActorScale().X + ScaleSpeed * DeltaTime);
	SetActorScale3D(FVector(Scale));

	if (TimeElapsed >= LifeDuration)
	{
		Destroy();
	}
}
