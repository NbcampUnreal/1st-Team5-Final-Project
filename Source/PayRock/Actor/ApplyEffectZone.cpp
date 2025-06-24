// PayRockGames

#include "ApplyEffectZone.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/Character/PRCharacter.h"

AApplyEffectZone::AApplyEffectZone()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bReplicates = true;
	bAlwaysRelevant = true;
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
	UE_LOG(LogTemp, Warning, TEXT("[%s] BeginPlay | Authority = %d | NetRole = %d"),
		*GetName(), HasAuthority(), (int32)GetLocalRole());

	PlaySoundIfValid();

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
	UE_LOG(LogTemp, Warning, TEXT("[%s] Destroyed | Authority = %d"), *GetName(), HasAuthority());

	if (HasAuthority())
	{
		TArray<AActor*> Targets;
		ActiveEffectMap.GenerateKeyArray(Targets);

		for (AActor* TargetActor : Targets)
		{
			RemoveEffectFromActor(TargetActor);
		}
	}
	if (IsValid(StaticMesh))
	{
		StaticMesh->SetVisibility(false, true);
		StaticMesh->DestroyComponent();
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

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

void AApplyEffectZone::Multicast_PlaySoundAtLocation_Implementation(USoundBase* Sound)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
	}
}

void AApplyEffectZone::PlaySoundIfValid()
{
	if (HasAuthority())
	{
		Multicast_PlaySoundAtLocation(ActivationSound);
	}
}