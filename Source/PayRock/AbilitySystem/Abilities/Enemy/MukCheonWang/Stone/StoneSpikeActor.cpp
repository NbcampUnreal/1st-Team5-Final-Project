#include "StoneSpikeActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Components/TimelineComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AStoneSpikeActor::AStoneSpikeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
    Collision->InitCapsuleSize(60.f, 100.f);
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
    RootComponent = Collision;

    VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
    VFX->SetupAttachment(RootComponent);

    MoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeline"));

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AStoneSpikeActor::OnSpikeOverlap);
}

void AStoneSpikeActor::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();

    if (TargetActor)
    {
        FVector Direction = (TargetActor->GetActorLocation() - StartLocation).GetSafeNormal();
        EndLocation = StartLocation + Direction * TravelDistance;
        SetActorRotation(Direction.Rotation());
    }
    else
    {
        EndLocation = StartLocation + GetActorForwardVector() * TravelDistance;
    }

    if (MoveCurve)
    {
        FOnTimelineFloat UpdateDelegate;
        UpdateDelegate.BindUFunction(this, FName("OnTimelineUpdate"));

        MoveTimeline->AddInterpFloat(MoveCurve, UpdateDelegate);
        MoveTimeline->SetLooping(false);
        MoveTimeline->SetTimelineLength(TravelDuration);
        MoveTimeline->PlayFromStart();
    }

    SetLifeSpan(TravelDuration + 1.0f);
}

void AStoneSpikeActor::OnTimelineUpdate(float Value)
{
    FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
    SetActorLocation(NewLocation);
}

void AStoneSpikeActor::OnSpikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
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

    Destroy();
}

void AStoneSpikeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (MoveTimeline) MoveTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}

void AStoneSpikeActor::SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility)
{
    InstigatorAbility = InAbility;
}

void AStoneSpikeActor::SetTarget(AActor* InTarget)
{
    TargetActor = InTarget;
}