#include "OrbLightActor.h"
#include "Components/PointLightComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "NiagaraComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AOrbLightActor::AOrbLightActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	OuterDamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OuterDamageSphere"));
	OuterDamageSphere->InitSphereRadius(600.f);
	OuterDamageSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	OuterDamageSphere->SetGenerateOverlapEvents(true);
	OuterDamageSphere->SetupAttachment(RootComponent);

	InnerSafeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSafeSphere"));
	InnerSafeSphere->InitSphereRadius(200.f);
	InnerSafeSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	InnerSafeSphere->SetGenerateOverlapEvents(true);
	InnerSafeSphere->SetupAttachment(RootComponent);

	LightSource = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	LightSource->SetupAttachment(RootComponent);
	LightSource->SetIntensity(8000.f);
	LightSource->SetAttenuationRadius(1000.f);

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetupAttachment(RootComponent);

	Ring = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Ring"));
	Ring->SetupAttachment(RootComponent);
	
	FixedScene = CreateDefaultSubobject<USceneComponent>(TEXT("FixedScene"));
	FixedScene->SetupAttachment(nullptr);
	
	MovementBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MovementBox"));
	MovementBox->SetBoxExtent(FVector(800.f, 800.f, 50.f));
	MovementBox->SetupAttachment(FixedScene);

	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void AOrbLightActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FVector SpawnLoc = GetActorLocation();
		FNavLocation NavLoc;

		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys && NavSys->ProjectPointToNavigation(SpawnLoc, NavLoc, FVector(500.f, 500.f, 1000.f)))
		{
			SetActorLocation(NavLoc.Location + FVector(0.f, 0.f, 80.f));
		}

		Multicast_PlayVFX();

		GetWorldTimerManager().SetTimer(RingVFXTimerHandle, this, &AOrbLightActor::Multicast_PlayRingVFX, 1.0f, true);
		MoveToRandomLocation();
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AOrbLightActor::ApplyLightSurvivalDOT, DamageTickInterval, true);
		GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AOrbLightActor::MoveToRandomLocation, MoveInterval, true);

		TArray<AActor*> FoundPlayers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APRCharacter::StaticClass(), FoundPlayers);
		for (AActor* Player : FoundPlayers)
		{
			if (APRCharacter* PR = Cast<APRCharacter>(Player))
			{
				CachedPlayers.Add(PR);
			}
		}
	}
}

void AOrbLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	if (bIsMoving)
	{
		FVector Current = GetActorLocation();
		FVector Next = FMath::VInterpConstantTo(Current, NextTargetLocation, DeltaTime, GetCurrentSpeed());
		SetActorLocation(Next, true);

		if (FVector::Dist(Next, NextTargetLocation) < 10.f)
		{
			bIsMoving = false;
		}
	}
}

void AOrbLightActor::ApplyLightSurvivalDOT()
{
	for (APRCharacter* Player : CachedPlayers)
	{
		if (!IsValid(Player)) continue;
		if (!OuterDamageSphere->IsOverlappingActor(Player)) continue;
		if (InnerSafeSphere->IsOverlappingActor(Player)) continue;
		ApplyDamageEffect(Player);
	}
}

void AOrbLightActor::MoveToRandomLocation()
{
	if (!IsValid(MovementBox)) return;

	FBox Bounds = MovementBox->Bounds.GetBox();
	FVector RandomLocation = FMath::RandPointInBox(Bounds);
	RandomLocation.Z = GetActorLocation().Z;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bBlocked = GetWorld()->LineTraceSingleByChannel(
		Hit,
		GetActorLocation(),
		RandomLocation,
		ECC_WorldStatic,
		Params
	);

	if (bBlocked) return;

	NextTargetLocation = RandomLocation;
	bIsMoving = true;
}

bool AOrbLightActor::IsPlayerInNavAndOutOfRange(APRCharacter* Player)
{
	if (!IsValid(Player)) return false;

	const float Dist = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
	if (Dist <= RequiredProximity) return false;

	FNavLocation Dummy;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) return false;

	return NavSys->ProjectPointToNavigation(Player->GetActorLocation(), Dummy, FVector(10.f));
}

float AOrbLightActor::GetCurrentSpeed() const
{
	const float Alpha = FMath::Clamp(ElapsedTime / OrbLifetime, 0.f, 1.f);
	return FMath::Lerp(MinSpeed, MaxSpeed, Alpha);
}

void AOrbLightActor::Multicast_PlayVFX_Implementation()
{
	Super::Multicast_PlayVFX_Implementation();

	if (LightSource)
	{
		LightSource->SetVisibility(true);
	}

	if (VFX && GetNetMode() != NM_DedicatedServer)
	{
		VFX->Activate(true);
	}
}

void AOrbLightActor::Multicast_PlayRingVFX_Implementation()
{
	if (Ring && GetNetMode() != NM_DedicatedServer)
	{
		Ring->Activate(true);
	}
}

void AOrbLightActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	GetWorldTimerManager().ClearTimer(MoveTimerHandle);
	GetWorldTimerManager().ClearTimer(RingVFXTimerHandle);

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AAIController* AICon = Cast<AAIController>(OwnerPawn->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool(TEXT("bIsSpecialPattern2"), false);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}
