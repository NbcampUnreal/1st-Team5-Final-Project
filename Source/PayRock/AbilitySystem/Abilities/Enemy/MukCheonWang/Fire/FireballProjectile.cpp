#include "FireballProjectile.h"

#include "FireDOTArea.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AFireballProjectile::AFireballProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(10.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = CollisionComponent;

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.6f;
    ProjectileMovement->InitialSpeed = 1200.f;
    ProjectileMovement->MaxSpeed = 1200.f;
}

void AFireballProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AFireballProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetActor)
    {
        float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
        if (Distance < 100.f)
        {
            HandleImpact();
        }
    }
}

void AFireballProjectile::InitTarget(AActor* InTarget)
{
    TargetActor = InTarget;
    if (TargetActor)
    {
        FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
    }
}

void AFireballProjectile::HandleImpact()
{
    if (DOTAreaClass)
    {
        FVector SpawnLoc = GetActorLocation() + FVector(0, 0, SpawnHeightOffset);
        GetWorld()->SpawnActor<AFireDOTArea>(DOTAreaClass, SpawnLoc, FRotator::ZeroRotator);
    }

    SetLifeSpan(ImpactDelay);
    SetActorTickEnabled(false);
} 
