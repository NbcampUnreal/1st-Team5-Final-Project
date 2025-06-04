#include "StoneSpikeActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AStoneSpikeActor::AStoneSpikeActor()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    
    Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
    Collision->InitCapsuleSize(60.f, 100.f);
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
    RootComponent = Collision;

    VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
    VFX->SetupAttachment(RootComponent);
    
    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
    MovementComp->InitialSpeed = 500.f;
    MovementComp->MaxSpeed = 500.f;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->ProjectileGravityScale = 0.f;

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AStoneSpikeActor::OnSpikeOverlap);
    
    BaseSpeed = 500.f;
}

void AStoneSpikeActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        Multicast_PlaySpikeVFX();
    }

    SetLifeSpan(15.f);
}


void AStoneSpikeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
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
    if (!HasAuthority()) return;

    Multicast_SetTarget(InTarget); 
}

void AStoneSpikeActor::Multicast_SetTarget_Implementation(AActor* InTarget)
{
    TargetActor = InTarget;

    if (MovementComp && TargetActor && TargetActor->GetRootComponent())
    {
        MovementComp->bIsHomingProjectile = true;
        MovementComp->HomingAccelerationMagnitude = 1000.f;
        MovementComp->HomingTargetComponent = TargetActor->GetRootComponent();
    }
}
void AStoneSpikeActor::Multicast_PlaySpikeVFX_Implementation()
{
    if (VFX)
    {
        VFX->Activate(true);
    }
}
