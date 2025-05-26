#include "HealingTreeActor.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "TimerManager.h"

AHealingTreeActor::AHealingTreeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

void AHealingTreeActor::BeginPlay()
{
    Super::BeginPlay();

    InitAbilityActorInfo();
    InitializeDefaultAttributes();

    AttributeSet = ASC ? ASC->GetSet<UPRAttributeSet>() : nullptr;

    GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &AHealingTreeActor::HealBoss, TickInterval, true);
}

void AHealingTreeActor::InitAbilityActorInfo()
{
    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
    }
}

void AHealingTreeActor::InitializeDefaultAttributes() const
{
    if (ASC)
    {
        FGameplayEffectContextHandle Context = ASC->MakeEffectContext();

        if (InitPrimaryAttributeEffect)
        {
            FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(InitPrimaryAttributeEffect, 1.f, Context);
            if (SpecHandle.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
        if (InitSecondaryAttributeEffect)
        {
            FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(InitSecondaryAttributeEffect, 1.f, Context);
            if (SpecHandle.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
        if (InitVitalAttributeEffect)
        {
            FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(InitVitalAttributeEffect, 1.f, Context);
            if (SpecHandle.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void AHealingTreeActor::HealBoss()
{
    if (Boss && Boss->GetAbilitySystemComponent())
    {
        UAbilitySystemComponent* BossASC = Boss->GetAbilitySystemComponent();

        FGameplayEffectContextHandle Context = BossASC->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = BossASC->MakeOutgoingSpec(nullptr, 1.f, Context);
        if (SpecHandle.IsValid())
        {
            BossASC->ApplyModToAttribute(AttributeSet->GetHealthAttribute(), EGameplayModOp::Additive, HealPerSecond);
        }
    }
}

void AHealingTreeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (AttributeSet && AttributeSet->GetHealth() <= 0.f)
    {
        Destroy();
    }
}

UAbilitySystemComponent* AHealingTreeActor::GetAbilitySystemComponent() const
{
    return ASC;
}

void AHealingTreeActor::SetBoss(AMukCheonWangCharacter* InBoss)
{
    Boss = InBoss;
}
