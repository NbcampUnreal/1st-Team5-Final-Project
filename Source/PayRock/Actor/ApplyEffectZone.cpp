// PayRockGames

#include "ApplyEffectZone.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "PayRock/Character/PRCharacter.h"

AApplyEffectZone::AApplyEffectZone()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMesh->SetCollisionObjectType(ECC_WorldDynamic);
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(1000.f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AApplyEffectZone::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* InstigatorPawn = GetInstigator();
	if (!IsValid(InstigatorPawn)) return;

	// Offset
	FVector ForwardVector = InstigatorPawn->GetActorForwardVector();
	ForwardVector.Z = 0.f;
	FVector Offset = ForwardVector.GetSafeNormal() * DistanceFromInstigator;
	if (ACharacter* Character = Cast<ACharacter>(InstigatorPawn))
	{
		if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
		{
			Offset.Z -= Capsule->GetScaledCapsuleHalfHeight();
		}
	}

	// Positioning
	if (bAttachToInstigator)
	{
		FAttachmentTransformRules AttachRules(
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		false);

		AttachToActor(InstigatorPawn, AttachRules);
		SetActorRelativeLocation(Offset);
	}
	else
	{
		SetActorLocation(InstigatorPawn->GetActorLocation() + Offset);
	}
}

void AApplyEffectZone::Destroyed()
{
	if (HasAuthority())
	{
		TArray<AActor*> Targets;
		ActiveEffectMap.GenerateKeyArray(Targets);

		for (AActor* TargetActor : Targets)
		{
			RemoveEffectFromActor(TargetActor);
		}
	}
	
	Super::Destroyed();
}

void AApplyEffectZone::ApplyEffectToActor(AActor* TargetActor)
{
	if (!HasAuthority()) return;
	
	if (!bShouldApplyToInstigator && TargetActor == GetInstigator()) return;
	if (bOnlyApplyToPlayerCharacters && !TargetActor->IsA<APRCharacter>()) return;
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		if (!ActiveEffectMap.Contains(TargetActor))
		{
			UAbilitySystemComponent* SourceASC =
				UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
			if (IsValid(SourceASC))
			{
				if (!EffectSpecHandle.IsValid()) return;
				FActiveGameplayEffectHandle ActiveEffectHandle =
					SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
				ActiveEffectMap.Add(TargetActor, ActiveEffectHandle);
			}
			else
			{
				if (!EffectSpecHandle.IsValid()) return;
				FActiveGameplayEffectHandle ActiveEffectHandle =
					TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
				ActiveEffectMap.Add(TargetActor, ActiveEffectHandle);
			}
		}	
	}
	else if (ActiveEffectMap.Contains(TargetActor))
	{
		ActiveEffectMap.Remove(TargetActor);
	}
}

void AApplyEffectZone::RemoveEffectFromActor(AActor* TargetActor)
{
	if (!HasAuthority()) return;
	if (!IsValid(TargetActor)) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetASC)) return;

	if (FActiveGameplayEffectHandle* HandlePtr = ActiveEffectMap.Find(TargetActor))
	{
		if (HandlePtr->IsValid())
		{
			TargetASC->RemoveActiveGameplayEffect(*HandlePtr);	
		}
		ActiveEffectMap.Remove(TargetActor);
	}
}
