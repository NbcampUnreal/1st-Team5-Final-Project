// PayRockGames

#include "GA_ClownAttack.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"

UGA_ClownAttack::UGA_ClownAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ClownAttack::ActivateAbility(
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

	AMarketClownMonster* Boss = Cast<AMarketClownMonster>(ActorInfo->AvatarActor.Get());
	if (!Boss)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimMontage* Montage = Boss->GetCurrentMaskAttackMontage();
	if (!Montage)
	{
		ResetBlackboardAttackState(Boss);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimInstance* Anim = Boss->GetMesh()->GetAnimInstance();
	if (!Anim)
	{
		ResetBlackboardAttackState(Boss);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	float PlayRate = 1.0f;
	if (const UPRAttributeSet* AS = Cast<UPRAttributeSet>(Boss->GetAttributeSet()))
	{
		PlayRate = FMath::Max(AS->GetAttackSpeed(), 0.2f);
	}

	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UGA_ClownAttack::OnMontageEnded);

	Anim->Montage_Play(Montage, PlayRate);
	Anim->Montage_SetEndDelegate(MontageEndDelegate, Montage);

	if (CooldownGameplayEffect)
	{
		ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_ClownAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AMarketClownMonster* Boss = Cast<AMarketClownMonster>(GetAvatarActorFromActorInfo()))
	{
		ResetBlackboardAttackState(Boss);
	}
}

void UGA_ClownAttack::ResetBlackboardAttackState(AMarketClownMonster* Boss)
{
	if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsBusy"), false);
		}
	}
}
