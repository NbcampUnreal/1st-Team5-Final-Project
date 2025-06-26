// PayRockGames


#include "BaseMontageDamageSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PayRock/PRGameplayTags.h"

void UBaseMontageDamageSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (IsValid(ActivationMontage) && bLockInputDuringMontage)
	{
		AActor* Avatar = GetAvatarActorFromActorInfo();
		if (APlayerController* PC = Cast<APlayerController>(Avatar->GetInstigatorController()))
		{
			Avatar->DisableInput(PC);
			CachedController = PC;
		}
	}

	if (IsValid(ActivationMontage))
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, ActivationMontage, MontageRate); 

		MontageTask->OnCompleted.AddDynamic(this, &UBaseMontageDamageSkill::OnMontageEnd);
		MontageTask->OnInterrupted.AddDynamic(this, &UBaseMontageDamageSkill::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UBaseMontageDamageSkill::OnMontageInterrupted);
		MontageTask->ReadyForActivation();

		float MontageLength = ActivationMontage->GetPlayLength();
		if (IsValid(InputBlockEffect))
		{
			UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InputBlockEffect, 1.f, Context);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
				Spec, FPRGameplayTags::Get().Player_Block_InputPressed, MontageLength);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
		}
	}

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, TriggerTag, nullptr, true, true);
	WaitEventTask->EventReceived.AddUniqueDynamic(this, &UBaseMontageDamageSkill::OnEventReceived);
	WaitEventTask->ReadyForActivation();
}

void UBaseMontageDamageSkill::OnMontageEnd()
{
	if (bLockInputDuringMontage && CachedController)
	{
		AActor* Avatar = GetAvatarActorFromActorInfo();
		if (IsValid(Avatar))
		{
			Avatar->EnableInput(CachedController);
		}
		CachedController = nullptr;
	}

	if (IsActive() && bEndAbilityOnMontageEnd)
	{
		K2_EndAbility();
	}
}

void UBaseMontageDamageSkill::OnMontageInterrupted()
{
	if (bLockInputDuringMontage && CachedController)
	{
		AActor* Avatar = GetAvatarActorFromActorInfo();
		if (IsValid(Avatar))
		{
			Avatar->EnableInput(CachedController);
		}
		CachedController = nullptr;
	}

	if (IsActive() && bEndAbilityOnMontageInterrupted)
	{
		K2_EndAbility();
	}
}

void UBaseMontageDamageSkill::OnEventReceived(FGameplayEventData Payload)
{
}
