#include "OrbLightActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "PayRock/Character/PRCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

AOrbLightActor::AOrbLightActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	SetRootComponent(DetectionSphere);
	DetectionSphere->InitSphereRadius(RequiredProximity);

	LightVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightVFX"));
	LightVFX->SetupAttachment(RootComponent);

	LightSource = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	LightSource->SetupAttachment(RootComponent);
	LightSource->SetIntensity(8000.f);
	LightSource->SetAttenuationRadius(1000.f);
}

void AOrbLightActor::BeginPlay()
{
	Super::BeginPlay();

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
		if (IsPlayerInNavAndOutOfRange(Player))
		{
			ApplyEffectToActor(Player);
		}
	}
}

void AOrbLightActor::MoveToRandomLocation()
{
	FNavLocation RandomPoint;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (NavSys && NavSys->GetRandomReachablePointInRadius(GetActorLocation(), MoveRadius, RandomPoint))
	{
		
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bBlocked = GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), RandomPoint.Location, ECC_WorldStatic, Params);

		if (bBlocked)
		{
			return;
		}

		NextTargetLocation = RandomPoint.Location;
		bIsMoving = true;
	}
}

void AOrbLightActor::ApplyEffectToActor(AActor* Actor)
{
	APRCharacter* PRChar = Cast<APRCharacter>(Actor);
	if (!IsValid(PRChar)) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
	{
		if (DamageEffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.f, TargetASC->MakeEffectContext());
			if (SpecHandle.IsValid())
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, Damage);
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				DamagedActors.Add(Actor);
			}
		}
	}
}

bool AOrbLightActor::IsPlayerInNavAndOutOfRange(APRCharacter* Player)
{
	if (!IsValid(Player)) return false;

	const float Dist = FVector::Dist(Player->GetActorLocation(), GetActorLocation());

	if (Dist <= RequiredProximity)
		return false;

	FNavLocation Dummy;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) return false;

	return NavSys->ProjectPointToNavigation(Player->GetActorLocation(), Dummy, FVector(10.f, 10.f, 10.f));
}

float AOrbLightActor::GetCurrentSpeed() const
{
	const float Alpha = FMath::Clamp(ElapsedTime / OrbLifetime, 0.f, 1.f);
	return FMath::Lerp(MinSpeed, MaxSpeed, Alpha);
}

void AOrbLightActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		AAIController* AICon = Cast<AAIController>(OwnerPawn->GetController());
		if (AICon && AICon->GetBlackboardComponent())
		{
			AICon->GetBlackboardComponent()->SetValueAsBool(TEXT("bIsSpecialPattern2"), false);
		}
	}

	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	GetWorldTimerManager().ClearTimer(MoveTimerHandle);
}