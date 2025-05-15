#include "GA_ChargeStamp.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
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

	StartChargeVisual(Avatar);

	/*FTimerHandle Timer;
	Avatar->GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([this, Avatar]()
	{
		JumpToTarget(Avatar);
	}), ChargeTime, false);*/
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
		// Enemy->PlayAnimMontage(Enemy->GetChargingMontage());
	}
}

void UGA_ChargeStamp::JumpToTarget(ACharacter* Avatar)
{
	if (!Avatar) return;

	float UpwardPower = 700.0f;
	FVector LaunchVel = FVector(0, 0, UpwardPower);
	Avatar->LaunchCharacter(LaunchVel, true, true);

	/*if (AGeneralSkyCharacter* Enemy = Cast<AGeneralSkyCharacter>(Avatar))
	{
		Enemy->PlayAnimMontage(Enemy->GetStampMontage());
	}*/
	
	/*FTimerHandle DamageTimer;
	Avatar->GetWorldTimerManager().SetTimer(DamageTimer, FTimerDelegate::CreateLambda([this, Avatar]()
	{
		ApplyStampDamage(Avatar);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}), 0.6f, false);*/
}

void UGA_ChargeStamp::ApplyStampDamage(ACharacter* Avatar)
{
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