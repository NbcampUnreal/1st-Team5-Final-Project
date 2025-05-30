// PayRockGames


#include "GA_Light.h"
#include "Kismet/GameplayStatics.h"
#include "OrbLightActor.h"
#include "LightSourceActor.h"
#include "TimerManager.h"

UGA_Light::UGA_Light()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Light"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_Light::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !OrbLightClass) return;
	
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
	FActorSpawnParameters Params;
	Params.Owner = OriginActor;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector SpawnLoc = OriginActor->GetActorLocation();
	const FRotator SpawnRot = OriginActor->GetActorRotation();

	SpawnedOrb = GetWorld()->SpawnActor<AOrbLightActor>(OrbLightClass, SpawnLoc, SpawnRot, Params);
}

void UGA_Light::EndLightSurvivalPattern()
{
	RestoreLights();

	if (IsValid(SpawnedOrb))
	{
		SpawnedOrb->Destroy();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
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
