#include "HealingTreeActor.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

AHealingTreeActor::AHealingTreeActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AHealingTreeActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &AHealingTreeActor::HealBoss, TickInterval, true);
}

void AHealingTreeActor::SetBoss(AMukCheonWangCharacter* InBoss)
{
	Boss = InBoss;
}

void AHealingTreeActor::HealBoss()
{
	if (!Boss || !Boss->GetAbilitySystemComponent()) return;

	if (const UPRAttributeSet* BossAttr = Cast<UPRAttributeSet>(Boss->GetAbilitySystemComponent()->GetAttributeSet(UPRAttributeSet::StaticClass())))
	{
		Boss->GetAbilitySystemComponent()->ApplyModToAttribute(
			BossAttr->GetHealthAttribute(),
			EGameplayModOp::Additive,
			HealPerSecond
		);
	}
}
