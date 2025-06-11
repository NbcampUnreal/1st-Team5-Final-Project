// PayRockGames


#include "GA_Light.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "OrbLightActor.h"
#include "LightSourceActor.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_Light::UGA_Light()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Light")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_Light::OnAuraEffectComplete()
{
	AActor* Avatar = AvatarActor.Get();
	if (!Avatar || !OrbLightClass)
	{
		FinishAbility();
		return;
	}

	
	DisableNearbyLights(Avatar);
	SpawnLightOrbAtCenter(Avatar);
	
	GetWorld()->GetTimerManager().SetTimer(
		EndPatternTimerHandle,
		this,
		&UGA_Light::EndLightSurvivalPattern,
		PatternDuration,
		false
	);
}

void UGA_Light::DisableNearbyLights(AActor* OriginActor)
{
	AffectedLightActors.Empty();

	TArray<AActor*> FoundLights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALightSourceActor::StaticClass(), FoundLights);

	for (AActor* Actor : FoundLights)
	{
		if (Actor && FVector::Dist(Actor->GetActorLocation(), OriginActor->GetActorLocation()) <= LightDetectionRadius)
		{
			if (ALightSourceActor* Light = Cast<ALightSourceActor>(Actor))
			{
				Light->TurnOffLight();
				AffectedLightActors.Add(Light);
			}
		}
	}
}

void UGA_Light::SpawnLightOrbAtCenter(AActor* OriginActor)
{
	if (!IsValid(OriginActor) || !OrbLightClass) return;

	FVector ForwardDir = OriginActor->GetActorForwardVector();
	FVector SpawnLoc = OriginActor->GetActorLocation() + ForwardDir * 800.f + FVector(0.f, 0.f, 150.f);
	FRotator SpawnRot = OriginActor->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = OriginActor;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnedOrb = GetWorld()->SpawnActor<AOrbLightActor>(OrbLightClass, SpawnLoc, SpawnRot, Params);
	
	if (SpawnedOrb.IsValid())
	{
		SpawnedOrb->InitializeEffectSource(this);
	}
}



void UGA_Light::EndLightSurvivalPattern()
{
	RestoreLights();
	if (SpawnedOrb.IsValid())
	{
		SpawnedOrb->Destroy();
		SpawnedOrb = nullptr;
	}
	if (AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor.Get()))
	{
		if (AAIController* AIController = Cast<AAIController>(Boss->GetController()))
		{
			if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bIsSpecialPattern2", false);
			}
		}
	}
	FinishAbility();
}


void UGA_Light::RestoreLights()
{
	for (ALightSourceActor* Light : AffectedLightActors)
	{
		if (IsValid(Light))
		{
			Light->RestoreOriginalLight();
		}
	}
	AffectedLightActors.Empty();
}