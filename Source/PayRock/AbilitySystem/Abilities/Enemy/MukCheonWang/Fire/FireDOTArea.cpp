#include "FireDOTArea.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"

AFireDOTArea::AFireDOTArea()
{
	PrimaryActorTick.bCanEverTick = true;

	AreaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AreaCollision"));
	AreaCollision->InitSphereRadius(0.f);
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &AFireDOTArea::OnActorEnter);
	RootComponent = AreaCollision;

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	VFX->SetupAttachment(RootComponent);
	VFX->SetRelativeScale3D(FVector(0.1f)); 
}

void AFireDOTArea::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Lifetime);


	AreaCollision->SetSphereRadius(0.f);
	CurrentRadius = 0.f;
	TimeElapsed = 0.f;

	GetWorld()->GetTimerManager().SetTimer(
		DamageStartTimerHandle, this, &AFireDOTArea::ApplyDotToOverlappingActors, 0.05f, false);
}

void AFireDOTArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;

	
	CurrentRadius = FMath::Min(MaxRadius, CurrentRadius + ExpansionSpeed * DeltaTime);
	AreaCollision->SetSphereRadius(CurrentRadius);

	float Scale = CurrentRadius / MaxRadius; 
	VFX->SetWorldScale3D(FVector(Scale));

	if (FMath::Fmod(TimeElapsed, DamageInterval) <= DeltaTime)
	{
		ApplyDotToOverlappingActors();
	}
}

void AFireDOTArea::ApplyDotToOverlappingActors()
{
	TArray<AActor*> Overlapping;
	AreaCollision->GetOverlappingActors(Overlapping);

	for (AActor* Actor : Overlapping)
	{
		ApplyEffectToActor(Actor);
	}
}

void AFireDOTArea::OnActorEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyEffectToActor(OtherActor);
}

void AFireDOTArea::ApplyEffectToActor(AActor* Actor)
{
	if (!Actor) return;
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
	{
		if (DamageEffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.f, TargetASC->MakeEffectContext());
			if (SpecHandle.IsValid())
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
					SpecHandle, DamageTypeTag, Damage);
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				DamagedActors.Add(Actor);
			}
		}
	}
}