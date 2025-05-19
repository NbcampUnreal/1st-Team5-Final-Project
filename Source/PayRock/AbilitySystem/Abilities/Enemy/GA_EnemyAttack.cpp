#include "GA_EnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Net/RepLayout.h"

UGA_EnemyAttack::UGA_EnemyAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_EnemyAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	float TimeRemaining = 0.f;
	float Duration = 0.f;
	GetCooldownTimeRemainingAndDuration(Handle, ActorInfo, TimeRemaining, Duration);
	if (TimeRemaining > 0.f)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(ActorInfo->AvatarActor.Get());
	if (!Enemy)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	
	if (UAnimMontage* Montage = Enemy->GetRandomAttackMontage())
	{
		UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded MontageEndDelegate;
			MontageEndDelegate.BindUObject(this, &UGA_EnemyAttack::OnMontageEnded);

			AnimInstance->Montage_Play(Montage);
			AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, Montage);
		}
		else
		{
			ResetBlackboardAttackState(Enemy);
		}
	}
	else
	{
		ResetBlackboardAttackState(Enemy);
	}

	if (CooldownGameplayEffect)
	{
		ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	}
	
}

void UGA_EnemyAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (Enemy)
	{
		ResetBlackboardAttackState(Enemy);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bInterrupted);
}


void UGA_EnemyAttack::ResetBlackboardAttackState(AEnemyCharacter* Enemy)
{
	if (AAIController* AICon = Cast<AAIController>(Enemy->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsBusy"), false);
		}
	}
}

void UGA_EnemyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) return;

	AController* Controller = Cast<AController>(ActorInfo->PlayerController.Get());
	if (!Controller)
	{
		APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
		if (Pawn) Controller = Pawn->GetController();
	}
	if (!Controller) return;

	UBlackboardComponent* BB = Controller->FindComponentByClass<UBlackboardComponent>();
	if (BB)
	{
		BB->SetValueAsBool(FName("bIsBusy"), false);
		BB->SetValueAsBool(FName("bIsAttacking"), false);
	}
}