#include "LightningStrikeActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


ALightningStrikeActor::ALightningStrikeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StrikeCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("StrikeCollision"));
    StrikeCollision->InitCapsuleSize(100.f, 200.f);
    StrikeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StrikeCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    RootComponent = StrikeCollision;

    LightningVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningVFX"));
    LightningVFX->SetupAttachment(RootComponent);
    LightningVFX->SetAutoActivate(false);

    StrikeCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightningStrikeActor::OnStrikeOverlap);
}

void ALightningStrikeActor::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Activate, this, &ALightningStrikeActor::ActivateStrike, DelayBeforeStrike, false);
    SetLifeSpan(DelayBeforeStrike + 1.f);
}

void ALightningStrikeActor::ActivateStrike()
{
    StrikeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LightningVFX->Activate(true);
}

void ALightningStrikeActor::OnStrikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                            bool bFromSweep, const FHitResult& SweepResult)
{
    if (!InstigatorAbility || !OtherActor) return;

    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
    {
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.f, ASC->MakeEffectContext());
        if (SpecHandle.IsValid())
        {
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, Damage);
            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void ALightningStrikeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALightningStrikeActor::SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility)
{
    InstigatorAbility = InAbility;
}
