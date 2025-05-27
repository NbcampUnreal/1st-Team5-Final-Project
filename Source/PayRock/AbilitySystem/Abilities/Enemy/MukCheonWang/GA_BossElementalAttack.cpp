// PayRockGames


#include "GA_BossElementalAttack.h"
// PayRockGames

#include "GA_BossElementalAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

UGA_BossElementalAttack::UGA_BossElementalAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_BossElementalAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!AttackMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	PlayMontage();
}

void UGA_BossElementalAttack::PlayMontage()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	ACharacter* Character = Cast<ACharacter>(Avatar);
	if (!Character || !AttackMontage) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		float Duration = AnimInstance->Montage_Play(AttackMontage);
		if (Duration > 0.f)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UGA_BossElementalAttack::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
		}
	}
}

void UGA_BossElementalAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bInterrupted, false);
}
