#include "FireballProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "PayRock/Character/PRCharacter.h"
#include "FireDOTArea.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); 
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 1200.f;
	ProjectileMovement->MaxSpeed = 1200.f;
	ProjectileMovement->SetActive(false);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.f);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireballProjectile::OnSphereOverlap);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnHit);
	
	GetWorld()->GetTimerManager().SetTimer(LaunchDelayHandle, [this]()
	{
		SetReplicateMovement(true);
		LaunchToTargetPlayer();
	}, FireRate, false);
}

void AFireballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FloatElapsedTime += DeltaTime;

	float OffsetZ = FMath::Sin(FloatElapsedTime * FloatSpeed) * FloatHeight * DeltaTime;
	AddActorWorldOffset(FVector(0.f, 0.f, OffsetZ));
	
}

void AFireballProjectile::LaunchToTargetPlayer()
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(GetOwner());
	if (!Boss) return;

	TArray<AActor*> Targets = Boss->GetDetectedActors();
	if (Targets.Num() == 0) return;

	AActor* Target = Targets[FMath::RandRange(0, Targets.Num() - 1)];
	if (!Target) return;

	FVector TargetLocation = Target->GetActorLocation() + FVector(0, 0, 0);
	LaunchVelocity = (TargetLocation - GetActorLocation()).GetSafeNormal() * ProjectileMovement->InitialSpeed;

	ProjectileMovement->Velocity = LaunchVelocity;
	ProjectileMovement->SetActive(true);
}


void AFireballProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner() || bHit) return;
	
	if (APRCharacter* PRChar = Cast<APRCharacter>(OtherActor))
	{
		bHit = true;

		if (HasAuthority() && DamageEffectClass)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
			UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

			if (TargetASC && SourceASC)
			{
				FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, SourceASC->MakeEffectContext());
				if (SpecHandle.IsValid())
				{
					SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, DamageAmount);
					SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
				}
			}
		}

		Destroy();
	}
}

void AFireballProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner() || bHit) return;
	bHit = true;
	
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

void AFireballProjectile::EnableReplication()
{
	SetReplicateMovement(true);
	LaunchToTargetPlayer();
}