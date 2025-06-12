#include "FireballProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FireDOTArea.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "PayRock/Character/PRCharacter.h"

AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 1200.f;
	ProjectileMovement->MaxSpeed = 1200.f;
	ProjectileMovement->SetActive(false);


	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->InitCapsuleSize(60.f, 100.f);
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CapsuleCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AFireballProjectile::OnEffectOverlap);
	CapsuleCollision->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnHit);

	SetRootComponent(CapsuleCollision);
	CollisionComponent = CapsuleCollision;
	
	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetupAttachment(RootComponent);
}



void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		LaunchDelayHandle,
		this,
		&AFireballProjectile::EnableReplication,
		FireRate,
		false
	);
}

void AFireballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FloatElapsedTime += DeltaTime;
	float OffsetZ = FMath::Sin(FloatElapsedTime * FloatSpeed) * FloatHeight * DeltaTime;
	AddActorWorldOffset(FVector(0.f, 0.f, OffsetZ));
}

void AFireballProjectile::EnableReplication()
{
	SetReplicateMovement(true);
	LaunchToTargetPlayer();
}

void AFireballProjectile::LaunchToTargetPlayer()
{
	if (AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(GetOwner()))
	{
		const TArray<TWeakObjectPtr<AActor>>& Targets = Boss->GetDetectedActors();
		if (Targets.IsEmpty()) return;

		AActor* Target = Targets[FMath::RandRange(0, Targets.Num() - 1)].Get();
		if (!Target) return;

		const FVector TargetLocation = Target->GetActorLocation();
		LaunchVelocity = (TargetLocation - GetActorLocation()).GetSafeNormal() * ProjectileMovement->InitialSpeed;

		ProjectileMovement->Velocity = LaunchVelocity;
		ProjectileMovement->SetActive(true);
	}
}

void AFireballProjectile::OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                          bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner() || bHit) return;

	if (Cast<APRCharacter>(OtherActor))
	{
		bHit = true;
		PlayImpactVFX();
		ApplyDamageEffect(OtherActor);
		Destroy();
	}
}

void AFireballProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner() || bHit) return;

	bHit = true;
	PlayImpactVFX();
	HandleImpact(true);
}

void AFireballProjectile::HandleImpact(bool bSpawnDOT)
{
	if (bSpawnDOT && DOTAreaClass)
	{
		FVector SpawnLoc = GetActorLocation() + FVector(0.f, 0.f, SpawnHeightOffset);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AFireDOTArea>(DOTAreaClass, SpawnLoc, FRotator::ZeroRotator, Params);
	}

	Destroy();
}

void AFireballProjectile::PlayImpactVFX()
{
	if (HasAuthority())
	{
		Multicast_PlayImpactVFX();
	}
}

void AFireballProjectile::Multicast_PlayImpactVFX_Implementation()
{
	if (ImpactVFX && GetWorld())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), FRotator::ZeroRotator);
	}
}
