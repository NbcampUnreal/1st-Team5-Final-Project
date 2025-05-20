#include "GA_ClownImeaAttack.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"

UGA_ClownImeaAttack::UGA_ClownImeaAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ClownImeaAttack::ActivateAbility(
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
	
	if (AAIController* AICon = Cast<AAIController>(Boss->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			if (BB->GetValueAsBool(FName("bIsAttacking")))
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				return;
			}
		}
	}
	
	AActor* Target = Cast<AActor>(Boss->GetBlackboardTarget());
	if (Target)
	{
		FVector DashDir = (Target->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
		DashDir.Z = 0.f;
		
		const float DashPower = 1200.f;
		
		Boss->AddMovementInput(DashDir, DashPower);
	}
	
	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC || !WeaponAttackAbility)
	{
		SafeEndAbility(true);
		return;
	}

	
	ASC->AbilityEndedCallbacks.AddUObject(this, &UGA_ClownImeaAttack::OnWeaponAbilityEnded);
	const bool bSuccess = ASC->TryActivateAbilityByClass(WeaponAttackAbility);

	if (!bSuccess)
	{
		SafeEndAbility(true);
	}
}

void UGA_ClownImeaAttack::OnWeaponAbilityEnded(UGameplayAbility* Ability)
{
	AMarketClownMonster* Boss = Cast<AMarketClownMonster>(GetAvatarActorFromActorInfo());
	if (!Boss) return;

	UAnimMontage* RoarMontage = Boss->GetRoarMontage();
	UAnimInstance* Anim = Boss->GetMesh()->GetAnimInstance();

	if (RoarMontage && Anim)
	{
		Anim->Montage_Play(RoarMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage*, bool bInterrupted)
		{
			SafeEndAbility(bInterrupted);
		});
		Anim->Montage_SetEndDelegate(EndDelegate, RoarMontage);
	}
	else
	{
		SafeEndAbility(false);
	}
}



void UGA_ClownImeaAttack::SafeEndAbility(bool bWasCancelled)
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
				BB->SetValueAsBool(FName("bIsAttacking"), false);
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bWasCancelled);
}
