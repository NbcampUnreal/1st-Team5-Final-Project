#include "GA_BossRoar.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UGA_BossRoar::UGA_BossRoar()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Roar")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Roaring"));
	ActiveRoarTag = FGameplayTag::RequestGameplayTag("Boss.State.Roaring");
}

void UGA_BossRoar::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Boss = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Boss || !RoarMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}



	AController* Controller = Cast<AController>(GetActorInfo().OwnerActor->GetInstigatorController());
	if (!Controller) return;

	UBlackboardComponent* BB = Controller->FindComponentByClass<UBlackboardComponent>();
	if (BB)
	{
		BB->SetValueAsBool(FName("bRoarDelay"), true);
		
		if (UWorld* World = Boss->GetWorld())
		{
			World->GetTimerManager().SetTimer(
				RoarDelayTimerHandle,
				this,
				&UGA_BossRoar::ResetRoarDelay,
				DelayTime, 
				false 
			);
		}
	}
	
	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, RoarMontage, 1.0f);

	Task->OnCompleted.AddDynamic(this, &UGA_BossRoar::OnRoarMontageEnded);
	Task->OnInterrupted.AddDynamic(this, &UGA_BossRoar::OnRoarMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_BossRoar::OnRoarMontageCancelled);
	Task->ReadyForActivation();
}

void UGA_BossRoar::OnRoarMontageEnded()
{
	ResetNearPlayerBlackboardFlag();

	
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_BossRoar::OnRoarMontageCancelled()
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_BossRoar::ResetNearPlayerBlackboardFlag()
{
	AController* Controller = Cast<AController>(GetActorInfo().OwnerActor->GetInstigatorController());
	if (!Controller) return;

	UBlackboardComponent* BB = Controller->FindComponentByClass<UBlackboardComponent>();
	if (BB)
	{
		BB->SetValueAsBool(FName("bIsNearPlayer"), false);
	}
}

void UGA_BossRoar::ResetRoarDelay()
{
	ACharacter* Boss = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Boss)
		return;

	AController* Controller = Cast<AController>(Boss->GetController());
	if (!Controller)
		return;

	UBlackboardComponent* BB = Controller->FindComponentByClass<UBlackboardComponent>();
	if (BB)
	{
		BB->SetValueAsBool(FName("bRoarDelay"), false);
	}
}
