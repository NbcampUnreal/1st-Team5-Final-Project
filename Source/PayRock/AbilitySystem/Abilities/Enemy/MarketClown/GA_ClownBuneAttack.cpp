#include "GA_ClownBuneAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"
#include "AbilitySystemComponent.h"

UGA_ClownBuneAttack::UGA_ClownBuneAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ClownBuneAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	bAbilityEnded = false;

	AMarketClownMonster* Boss = Cast<AMarketClownMonster>(ActorInfo->AvatarActor.Get());
	if (!Boss)
	{
		SafeEndAbility(true);
		return;
	}

	AAIController* AICon = Cast<AAIController>(Boss->GetController());
	if (!AICon)
	{
		SafeEndAbility(true);
		return;
	}

	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (!BB)
	{
		SafeEndAbility(true);
		return;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
	if (!Target || !Target->IsValidLowLevelFast())
	{
		SafeEndAbility(true);
		return;
	}

	const float Distance = FVector::Dist(Boss->GetActorLocation(), Target->GetActorLocation());

	if (Distance > 400.f)
	{
		FAIMoveRequest MoveReq;
		MoveReq.SetGoalActor(Target);
		MoveReq.SetAcceptanceRadius(300.f);

		FNavPathSharedPtr NavPath;
		FPathFollowingRequestResult MoveResult = AICon->MoveTo(MoveReq, &NavPath);

		if (!AICon->ReceiveMoveCompleted.IsAlreadyBound(this, &UGA_ClownBuneAttack::OnMoveCompleted))
		{
			AICon->ReceiveMoveCompleted.AddDynamic(this, &UGA_ClownBuneAttack::OnMoveCompleted);
		}
	}
	else
	{
		TryActivateAttackAbility(Boss);
	}
}

void UGA_ClownBuneAttack::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	AMarketClownMonster* Boss = Cast<AMarketClownMonster>(GetAvatarActorFromActorInfo());
	if (!Boss)
	{
		SafeEndAbility(true);
		return;
	}

	if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
	{
		AICon->ReceiveMoveCompleted.RemoveDynamic(this, &UGA_ClownBuneAttack::OnMoveCompleted);
	}

	if (Result == EPathFollowingResult::Success)
	{
		TryActivateAttackAbility(Boss);
	}
	else
	{
		SafeEndAbility(true);
	}
}

void UGA_ClownBuneAttack::TryActivateAttackAbility(AMarketClownMonster* Boss)
{
	if (!Boss)
	{
		SafeEndAbility(true);
		return;
	}

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC || !WeaponCollisionAbility)
	{
		SafeEndAbility(true);
		return;
	}
	
	if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			AActor* Target = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
			if (Target)
			{
				FVector BossLoc = Boss->GetActorLocation();
				FVector TargetLoc = Target->GetActorLocation();

				FRotator LookAtRot = (TargetLoc - BossLoc).Rotation();
				LookAtRot.Pitch = 0.f;
				LookAtRot.Roll = 0.f;

				Boss->SetActorRotation(LookAtRot);
			}
		}
	}

	const bool bSuccess = ASC->TryActivateAbilityByClass(WeaponCollisionAbility);
	
	if (bSuccess)
	{
		if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool(FName("bIsFlee"), true);
			}
		}
	}

	SafeEndAbility(!bSuccess);
}



void UGA_ClownBuneAttack::SafeEndAbility(bool bWasCancelled)
{
	if (bAbilityEnded) return;
	bAbilityEnded = true;

	if (AMarketClownMonster* Boss = Cast<AMarketClownMonster>(GetAvatarActorFromActorInfo()))
	{
		if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool(FName("bIsBusy"), false);
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bWasCancelled);
}
