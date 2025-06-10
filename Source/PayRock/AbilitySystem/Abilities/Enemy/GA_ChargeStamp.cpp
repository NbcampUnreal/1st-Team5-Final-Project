#include "GA_ChargeStamp.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Enemy/SpecialEnemy/GeneralSky/GeneralSkyCharacter.h"

UGA_ChargeStamp::UGA_ChargeStamp()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ChargeStamp::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Avatar = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Avatar) return;

	if (const AActor* RawTarget = TriggerEventData ? TriggerEventData->Target : nullptr)
	{
		TargetLocation = RawTarget->GetActorLocation();
	}
	else
	{
		TargetLocation = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * 300.f;
	}

	//StartChargeVisual(Avatar);

}

void UGA_ChargeStamp::StartChargeVisual(AActor* Avatar)
{
	if (AGeneralSkyCharacter* Enemy = Cast<AGeneralSkyCharacter>(Avatar))
	{
		if (AAIController* AICon = Cast<AAIController>(Enemy->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bIsAttacking", true);
				AICon->StopMovement();
			}
		}
		Enemy->NetMulticast_StartChargingVisual(ChargeTime);
	}
}

void UGA_ChargeStamp::JumpToTarget(ACharacter* Avatar)
{
	if (!Avatar) return;

	float UpwardPower = 700.0f;
	FVector LaunchVel = FVector(0, 0, UpwardPower);
	Avatar->LaunchCharacter(LaunchVel, true, true);

}

void UGA_ChargeStamp::ApplyStampDamage(ACharacter* Avatar)
{

	FVector Start = Avatar->GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000.f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Avatar);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_WorldStatic,
		Params
	);

	FVector SpawnLocation = bHit ? HitResult.ImpactPoint + FVector(0, 0, 50.f) : Avatar->GetActorLocation();

	if (AuraEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			AuraEffect,
			SpawnLocation,
			FRotator::ZeroRotator,
			FVector(2, 2, 2)
		);
	}
	UKismetSystemLibrary::DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FLinearColor::Red,
		5.0f,
		5.0f
		);
	
	TArray<AActor*> HitActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APRCharacter::StaticClass(), HitActors);

	for (AActor* Target : HitActors)
	{
		if (FVector::Dist(Target->GetActorLocation(), Avatar->GetActorLocation()) <= DamageRadius)
		{
			CauseDamage(Target);
		}
	}
	if (AGeneralSkyCharacter* Enemy = Cast<AGeneralSkyCharacter>(Avatar))
	{
		
		if (AAIController* AICon = Cast<AAIController>(Enemy->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bIsAttacking", false);
				BB->SetValueAsBool("bInAttackRange", false);
			}
		}
	}
}