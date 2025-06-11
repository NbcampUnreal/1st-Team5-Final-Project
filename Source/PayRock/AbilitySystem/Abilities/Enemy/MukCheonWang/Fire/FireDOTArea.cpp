#include "FireDOTArea.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"

AFireDOTArea::AFireDOTArea()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleComponent->InitCapsuleSize(InitialRadius, InitialHalfHeight);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireDOTArea::OnEffectOverlap);

	SetRootComponent(CapsuleComponent);
	CollisionComponent = CapsuleComponent;

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetupAttachment(RootComponent);
}

void AFireDOTArea::BeginPlay()
{
	Super::BeginPlay();

	CurrentRadius = 0.f;
	CurrentHeight = 0.f;
	TimeElapsed = 0.f;

	CapsuleComponent->SetCapsuleSize(0.f, 0.f);

	GetWorld()->GetTimerManager().SetTimer(DamageStartTimerHandle, this, &AFireDOTArea::ApplyDotToOverlappingActors, 0.05f, false);
}

void AFireDOTArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;

	CurrentRadius = FMath::Min(MaxRadius, CurrentRadius + ExpansionSpeed * DeltaTime);
	CurrentHeight = FMath::Min(MaxHalfHeight, CurrentHeight + ExpansionSpeed * DeltaTime * 0.5f);

	CapsuleComponent->SetCapsuleSize(CurrentRadius, CurrentHeight);

	if (HasAuthority())
	{
		float Scale = CurrentRadius / MaxRadius;
		Multicast_SetVFXScale(Scale);
	}

	if (FMath::Fmod(TimeElapsed, DamageInterval) <= DeltaTime)
	{
		ApplyDotToOverlappingActors();
	}
}

void AFireDOTArea::OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyEffectToActor(OtherActor);
}

void AFireDOTArea::ApplyDotToOverlappingActors()
{
	TArray<AActor*> OverlappingActors;
	CapsuleComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		ApplyEffectToActor(Actor);
	}
}

void AFireDOTArea::ApplyEffectToActor(AActor* Actor)
{
	if (!Actor || DamagedActors.Contains(Actor)) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
	{
		if (DamageEffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.f, TargetASC->MakeEffectContext());
			if (SpecHandle.IsValid())
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, DamageAmount);
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				DamagedActors.Add(Actor);
				
				FTimerHandle ClearHandle;
				FTimerDelegate ClearDelegate;
				ClearDelegate.BindLambda([=, this]()
				{
					DamagedActors.Remove(Actor);
				});
				GetWorld()->GetTimerManager().SetTimer(ClearHandle, ClearDelegate, DamageInterval, false);

			}
		}
	}
}

void AFireDOTArea::Multicast_SetVFXScale_Implementation(float Scale)
{
	if (VFX)
	{
		VFX->SetWorldScale3D(FVector(Scale));
	}
}
