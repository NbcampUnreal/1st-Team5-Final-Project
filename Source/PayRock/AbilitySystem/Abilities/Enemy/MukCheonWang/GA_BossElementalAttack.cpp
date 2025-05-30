// PayRockGames


#include "GA_BossElementalAttack.h"
// PayRockGames

#include "GA_BossElementalAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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

	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;

	
	if (!AttackMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	//PlayMontage();


	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			AttackMontage,
			1.0f,
			NAME_None,
			false
		);

	MontageTask->OnCompleted.AddDynamic(this, &UGA_BossElementalAttack::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_BossElementalAttack::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this,&UGA_BossElementalAttack::OnMontageInterrupt);
	MontageTask->ReadyForActivation();
	
}

void UGA_BossElementalAttack::Multicast_PlayMontage_Implementation(UAnimMontage* MontageToPlay)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		float Duration = AnimInstance->Montage_Play(MontageToPlay);
		if (Duration > 0.f && Character->HasAuthority())
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UGA_BossElementalAttack::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
		}
	}
}

void UGA_BossElementalAttack::PlayMontage()
{
	// AActor* Avatar = GetAvatarActorFromActorInfo();
	// if (!Avatar) return;
	//
	// ACharacter* Character = Cast<ACharacter>(Avatar);
	// if (!Character || !AttackMontage) return;
	//
	//
	// UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	// if (AnimInstance)
	// {
	// 	float Duration = AnimInstance->Montage_Play(AttackMontage);
	// 	if (Duration > 0.f)
	// 	{
	// 		FOnMontageEnded EndDelegate;
	// 		EndDelegate.BindUObject(this, &UGA_BossElementalAttack::OnMontageEnded);
	// 		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	// 	}
	// }

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !AttackMontage) return;

	if (Avatar->HasAuthority())
	{
		Multicast_PlayMontage(AttackMontage);
	}
}

void UGA_BossElementalAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Display, TEXT("OnMontageEnded"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bInterrupted, false);
}

void UGA_BossElementalAttack::OnMontageCompleted()
{
	UE_LOG(LogTemp, Display, TEXT("OnMontageEnded"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_BossElementalAttack::OnMontageCancelled()
{
	UE_LOG(LogTemp, Display, TEXT("OnMontageEnded"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_BossElementalAttack::OnMontageInterrupt()
{
	UE_LOG(LogTemp, Display, TEXT("OnMontageEnded"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
