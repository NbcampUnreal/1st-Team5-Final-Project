#include "WaterWave.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/Character/PRCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GeometryCacheComponent.h"
#include "GeometryCache.h"

AWaterWave::AWaterWave()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	CollisionBox->InitBoxExtent(FVector(100.f, 300.f, 100.f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWaterWave::OnOverlapBegin);

	GeometryCacheComp = CreateDefaultSubobject<UGeometryCacheComponent>(TEXT("GeometryCacheComp"));
	GeometryCacheComp->SetupAttachment(RootComponent);
	GeometryCacheComp->SetRelativeLocation(FVector::ZeroVector); // 필요 시 조정
	GeometryCacheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GeometryCacheComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void AWaterWave::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(Lifetime);
	MoveDirection = GetActorForwardVector().GetSafeNormal();

	Multicast_PlayVFX();
}

void AWaterWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DeltaMove = MoveDirection * MoveSpeed * DeltaTime;
	AddActorWorldOffset(DeltaMove, true);
}

void AWaterWave::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								bool bFromSweep, const FHitResult& SweepResult)
{
	if (APRCharacter* Character = Cast<APRCharacter>(OtherActor))
	{
		FVector KnockbackDir = MoveDirection;
		KnockbackDir.Z = 0.5f;
		KnockbackDir.Normalize();

		Character->LaunchCharacter(KnockbackDir * KnockbackStrength, true, true);
	}
}

void AWaterWave::Multicast_PlayVFX_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer || !GeometryCacheAsset) return;

	GeometryCacheComp->SetGeometryCache(GeometryCacheAsset);
	GeometryCacheComp->Play();
}

void AWaterWave::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
