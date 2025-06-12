// LightningStrikeActor.cpp

#include "LightningStrikeActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "AbilitySystemBlueprintLibrary.h"

ALightningStrikeActor::ALightningStrikeActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    StrikeCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("StrikeCollision"));
    StrikeCollision->InitCapsuleSize(100.f, 200.f);
    StrikeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StrikeCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    SetRootComponent(StrikeCollision);

    CollisionComponent = StrikeCollision;

    LightningVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningVFX"));
    LightningVFX->SetupAttachment(RootComponent);
    LightningVFX->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
    LightningVFX->SetAutoActivate(false);
}

void ALightningStrikeActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle_Activate, this, &ALightningStrikeActor::ActivateStrike, DelayBeforeStrike, false);
    }

    SetLifeSpan(DelayBeforeStrike + 1.f);
}

void ALightningStrikeActor::ActivateStrike()
{
    if (StrikeCollision)
    {
        StrikeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        StrikeCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightningStrikeActor::OnEffectOverlap);
    }

    Multicast_PlayLightningVFX();
}

void ALightningStrikeActor::OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                            bool bFromSweep, const FHitResult& SweepResult)
{
    ApplyDamageEffect(OtherActor);
}

void ALightningStrikeActor::Multicast_PlayLightningVFX_Implementation()
{
    if (LightningVFX && GetNetMode() != NM_DedicatedServer)
    {
        LightningVFX->Activate(true);
    }
}
