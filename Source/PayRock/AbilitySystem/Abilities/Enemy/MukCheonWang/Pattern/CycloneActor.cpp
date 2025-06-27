#include "CycloneActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GeometryCacheComponent.h"
#include "GeometryCache.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/Lightning/LightningStrikeActor.h"
#include "PayRock/Character/PRCharacter.h"

ACycloneActor::ACycloneActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);
	PullRange = CreateDefaultSubobject<USphereComponent>(TEXT("PullRange"));
	PullRange->InitSphereRadius(600.f);
	PullRange->SetCollisionProfileName(TEXT("OverlapAll"));
	PullRange->SetGenerateOverlapEvents(true);
	PullRange->SetupAttachment(RootComponent);
	PullRange->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACycloneActor::OnOverlapBegin);
	PullRange->OnComponentEndOverlap.AddUniqueDynamic(this, &ACycloneActor::OnOverlapEnd);
	
	GeometryCacheComp = CreateDefaultSubobject<UGeometryCacheComponent>(TEXT("CycloneVFX"));
	GeometryCacheComp->SetupAttachment(RootComponent);
	GeometryCacheComp->SetRelativeLocation(FVector::ZeroVector);
	GeometryCacheComp->SetRelativeScale3D(FVector(0.5f));
	GeometryCacheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GeometryCacheComp->SetVisibility(false);
	GeometryCacheComp->SetComponentTickEnabled(false);
}



void ACycloneActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FTimerHandle VFXDelayHandle;
		GetWorld()->GetTimerManager().SetTimer(
			VFXDelayHandle,
			this,
			&ACycloneActor::Multicast_PlayVFX,
			1.0f, 
			false
		);

		SpawnLightning();

		TArray<AActor*> InitialOverlaps;
		PullRange->GetOverlappingActors(InitialOverlaps, APRCharacter::StaticClass());

		for (AActor* Actor : InitialOverlaps)
		{
			if (APRCharacter* Player = Cast<APRCharacter>(Actor))
			{
				OverlappingPlayers.AddUnique(Player);
			}
		}
	}
}



void ACycloneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (APRCharacter* Player : OverlappingPlayers)
	{
		if (!IsValid(Player)) continue;

		FVector Direction = (GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();
		FVector PullVelocity = Direction * PullStrength * DeltaTime;

		if (ACharacter* Char = Cast<ACharacter>(Player))
		{
			Char->AddMovementInput(Direction, PullStrength * DeltaTime);
		}
	}
}

void ACycloneActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                   bool bFromSweep, const FHitResult& SweepResult)
{
	if (APRCharacter* Player = Cast<APRCharacter>(OtherActor))
	{
		OverlappingPlayers.AddUnique(Player);
	}
}

void ACycloneActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APRCharacter* Player = Cast<APRCharacter>(OtherActor))
	{
		OverlappingPlayers.Remove(Player);
	}
}

void ACycloneActor::SpawnLightning()
{
	if (!LightningClass || !IsValid(this)) return;

	const FVector RandOffset = FMath::VRand().GetSafeNormal() * FMath::RandRange(0.f, LightningRadius);
	const FVector SpawnLoc = GetActorLocation() + RandOffset;

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ALightningStrikeActor* Lightning = GetWorld()->SpawnActor<ALightningStrikeActor>(LightningClass, SpawnLoc, FRotator::ZeroRotator, Params);
	if (Lightning && InstigatorAbility.IsValid())
	{
		Lightning->InitializeEffectSource(InstigatorAbility.Get());
	}

	const float NextInterval = FMath::FRandRange(MinInterval, MaxInterval);
	GetWorld()->GetTimerManager().SetTimer(LightningLoopTimer, this, &ACycloneActor::SpawnLightning, NextInterval, false);
}


void ACycloneActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(LightningLoopTimer);

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AAIController* AICon = Cast<AAIController>(OwnerPawn->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool(TEXT("bIsSpecialPattern1"), false);
			}
		}
	}

	if (OnCycloneDestroyed.IsBound())
	{
		OnCycloneDestroyed.Execute();
	}

	Super::EndPlay(EndPlayReason);
}

void ACycloneActor::Multicast_PlayVFX_Implementation()
{
	if (GetNetMode() == NM_DedicatedServer) return;

	if (GeometryCacheAsset)
	{
		GeometryCacheComp->SetGeometryCache(GeometryCacheAsset);
		GeometryCacheComp->SetLooping(true);
		GeometryCacheComp->ResetAnimationTime();
		GeometryCacheComp->SetVisibility(true);
		GeometryCacheComp->SetComponentTickEnabled(true);
		GeometryCacheComp->Play();
		
	}


	if (VFX)
	{
		VFX->Activate(true);
	}
}

void ACycloneActor::InitializeEffectSource(UGameplayAbility* InAbility)
{
	InstigatorAbility = InAbility;
}

void ACycloneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACycloneActor, GeometryCacheAsset);
}