#include "FireDOTArea.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AFireDOTArea::AFireDOTArea()
{
	PrimaryActorTick.bCanEverTick = true;

	AreaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AreaCollision"));
	AreaCollision->InitSphereRadius(0.f);
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	RootComponent = AreaCollision;

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	VFX->SetupAttachment(RootComponent);
}

void AFireDOTArea::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Lifetime);
}

void AFireDOTArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;
	CurrentRadius = FMath::Min(MaxRadius, CurrentRadius + ExpansionSpeed * DeltaTime);
	AreaCollision->SetSphereRadius(CurrentRadius);

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
		if (!Actor || DamagedActors.Contains(Actor)) continue;

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
}
