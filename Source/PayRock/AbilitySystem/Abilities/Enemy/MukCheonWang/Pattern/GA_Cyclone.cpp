#include "GA_Cyclone.h"
#include "CycloneActor.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_Cyclone::UGA_Cyclone()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Cyclone")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}


void UGA_Cyclone::OnAuraEffectComplete()
{
	if (!AvatarActor.IsValid() || !CycloneClass) return;

	const FVector ForwardDir = AvatarActor->GetActorForwardVector();
	const FVector InitialLoc = AvatarActor->GetActorLocation() + ForwardDir * ForwardOffset;

	FVector ProjectedSpawnLoc = InitialLoc;
	FNavLocation NavLoc;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys && NavSys->ProjectPointToNavigation(InitialLoc, NavLoc, FVector(500.f, 500.f, 1000.f)))
	{
		ProjectedSpawnLoc = NavLoc.Location;
	}
	else
	{
		ProjectedSpawnLoc.Z = AvatarActor->GetActorLocation().Z;
	}

	ProjectedSpawnLoc.Z += 20.f;

	FActorSpawnParameters Params;
	Params.Owner = AvatarActor.Get();
	Params.Instigator = AvatarActor->GetInstigator();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SpawnedCyclone = GetWorld()->SpawnActor<ACycloneActor>(CycloneClass, ProjectedSpawnLoc, AvatarActor->GetActorRotation(), Params);

	if (SpawnedCyclone.IsValid())
	{
		SpawnedCyclone->InitializeEffectSource(this);
	}

	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&UGA_Cyclone::DestroyCyclone,
		CycloneDuration,
		false
	);
}


void UGA_Cyclone::DestroyCyclone()
{
	if (SpawnedCyclone.IsValid())
	{
		SpawnedCyclone->Destroy();
	}

	OnCycloneEnded();
}

void UGA_Cyclone::OnCycloneEnded()
{
	if (AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor.Get()))
	{
		if (AAIController* AIController = Cast<AAIController>(Boss->GetController()))
		{
			if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
			{
				BB->SetValueAsBool(FName("bIsSpecialPattern1"), false);
			}
		}
	}

	FinishAbility();
}
