#include "StoneSpikeActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
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

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AStoneSpikeActor::OnSpikeOverlap);
    
    BaseSpeed = 500.f;
}

void AStoneSpikeActor::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(15.f);
}

void AStoneSpikeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TargetActor) return;

    FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector DeltaMove = Direction * BaseSpeed * DeltaTime;

    SetActorLocation(GetActorLocation() + DeltaMove);
    SetActorRotation(Direction.Rotation());
    
}

void AStoneSpikeActor::OnSpikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AStoneSpikeActor::SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility)
{
    InstigatorAbility = InAbility;
}

void AStoneSpikeActor::SetTarget(AActor* InTarget)
{
    TargetActor = InTarget;
}
