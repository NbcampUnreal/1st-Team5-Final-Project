// PayRockGames


#include "MukCheonWangCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "AIController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Enemy/EBossPhase.h"


AMukCheonWangCharacter::AMukCheonWangCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 3000.f;
    SightConfig->LoseSightRadius = 3200.f;
    SightConfig->PeripheralVisionAngleDegrees = 360.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 5000.f;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->ConfigureSense(*HearingConfig);
    AIPerception->ConfigureSense(*DamageConfig);
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

    
    ChairMesh = CreateDefaultSubobject<USkeletalMeshComponent>("KingChair");
    ChairMesh->SetupAttachment(GetMesh());
    ChairMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    
    KingCrown = CreateDefaultSubobject<USkeletalMeshComponent>("KingCrown");
    KingCrown->SetupAttachment(GetMesh(),CrownSocketName);
    KingCrown->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    
    SetReplicates(true);
}

void AMukCheonWangCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerception)
    {
        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMukCheonWangCharacter::OnTargetPerceptionUpdated);
    }

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMukCheonWangCharacter::UpdateRandomTarget, 3.0f, true, 3.0f);
}

void AMukCheonWangCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const float HPPercent = GetHealthPercent();

    EBossPhase NewPhase = EBossPhase::Phase1;
    if (HPPercent <= 0.3f)
        NewPhase = EBossPhase::Phase3;
    else if (HPPercent <= 0.6f)
        NewPhase = EBossPhase::Phase2;
    

    if (NewPhase != CurrentPhase)
    {
        CurrentPhase = NewPhase;
        OnPhaseChanged(NewPhase);
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
            {
                BB->SetValueAsInt(TEXT("CurrentPhase_Int"), static_cast<int32>(NewPhase));
                
                if (NewPhase == EBossPhase::Phase2)
                {
                    BB->SetValueAsBool(TEXT("bIsSpecialPattern1"), true);
                    ToggleVisibleChairMesh(false);
                }
                
                if (NewPhase == EBossPhase::Phase3)
                {
                    BB->SetValueAsBool(TEXT("bIsSpecialPattern2"), true);
                }
            }
        }
    }
}

void AMukCheonWangCharacter::ToggleVisibleChairMesh(bool isActive)
{
    ChairMesh->SetVisibility(isActive);
}

void AMukCheonWangCharacter::PerformMeleeSweep(FName SocketName, UBaseDamageGameplayAbility* Ablilty)
{
    if (!GetMesh()) return;

    FVector Start = GetMesh()->GetSocketLocation(SocketName);
    FVector End = Start + GetActorForwardVector() * AttackRange;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.bReturnPhysicalMaterial = false;

    TArray<FHitResult> HitResults;
    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(AttackRadius),
        Params
    );

    //이 부분 공격 GA-에 들어가서 켜주고 애니메이션 끝나는지점에 Clear해주기
    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor && !HitActors.Contains(HitActor))
            {
                HitActors.Add(HitActor);

                if (HitActor && !HitActors.Contains(HitActor))
                {
                    HitActors.Add(HitActor);
                    Ablilty->CauseDamage(HitActor);
                }
            }
        }
    }

    // Debug Sphere
    //에디터에서만 디버깅용도
#if WITH_EDITOR
    DrawDebugSphere(GetWorld(), End, AttackRadius, 12, FColor::Red, false, 1.0f);
#endif
}

void AMukCheonWangCharacter::ClearHitActors()
{
    HitActors.Empty();
   
}

void AMukCheonWangCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor || !Actor->IsA(APRCharacter::StaticClass())) return;

    APRCharacter* Player = Cast<APRCharacter>(Actor);
    if (!Player || Player->GetbIsDead() || Player->GetbIsInvisible() || Player->GetbIsExtracted())
    {
        return;
    }

    const FVector AIPos = GetActorLocation();
    const FAISenseID HearingID = UAISense::GetSenseID<UAISense_Hearing>();
    const FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
    const FAISenseID DamageID = UAISense::GetSenseID<UAISense_Damage>();

    AAIController* AICon = Cast<AAIController>(GetController());
    if (!AICon || !AICon->GetBlackboardComponent()) return;

    UBlackboardComponent* BB = AICon->GetBlackboardComponent();

    if (Stimulus.Type == HearingID)
    {
        if (Stimulus.Strength >= LoudnessThreshold)
        {
            BB->SetValueAsObject(TEXT("TargetActor"), Actor);
            BB->SetValueAsBool(TEXT("bPlayerDetect"), true);
        }
        else if (Stimulus.Strength >= MinLoudnessToReact)
        {
            FRotator LookRot = (Stimulus.StimulusLocation - AIPos).Rotation();
            SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
        }
    }
    else if (Stimulus.Type == SightID || Stimulus.Type == DamageID)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            if (!DetectedActors.Contains(Actor))
            {
                DetectedActors.Add(Actor);
            }

            BB->SetValueAsObject(TEXT("TargetActor"), Actor);
            BB->SetValueAsBool(TEXT("bPlayerDetect"), true);
        }
        else
        {
            DetectedActors.Remove(Actor);
            
            UObject* CurrentTarget = BB->GetValueAsObject(TEXT("TargetActor"));
            if (CurrentTarget == Actor)
            {
                BB->ClearValue(TEXT("TargetActor"));
                BB->SetValueAsBool(TEXT("bPlayerDetect"), false);
            }
        }
    }
}

void AMukCheonWangCharacter::UpdateRandomTarget()
{
    DetectedActors.Empty();

    TArray<AActor*> AllDetected;
    AIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), AllDetected);

    for (AActor* Actor : AllDetected)
    {
        if (APRCharacter* Player = Cast<APRCharacter>(Actor))
        {
            if (!Player->GetbIsDead() && !Player->GetbIsInvisible() && !Player->GetbIsExtracted())
            {
                DetectedActors.Add(Player);
            }
        }
    }

    if (DetectedActors.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, DetectedActors.Num() - 1);
        AActor* ChosenTarget = DetectedActors[Index].Get();

        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
            {
                BB->SetValueAsObject(TEXT("TargetActor"), ChosenTarget);
                BB->SetValueAsBool(TEXT("bPlayerDetect"), true);
            }
        }
    }
    else
    {
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
            {
                BB->ClearValue(TEXT("TargetActor"));
                BB->SetValueAsBool(TEXT("bPlayerDetect"), false);
            }
        }
    }
}


void AMukCheonWangCharacter::OnPhaseChanged(EBossPhase NewPhase)
{
    
    switch (NewPhase)
    {
    case EBossPhase::Phase2:
            break;
    case EBossPhase::Phase3:
            break;
    default:
        break;
    }
}

float AMukCheonWangCharacter::GetHealthPercent() const
{
    const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (!ASC) return 0.f;

    const float CurrentHealth = ASC->GetNumericAttribute(UPRAttributeSet::GetHealthAttribute());
    const float MaxHealth = ASC->GetNumericAttribute(UPRAttributeSet::GetMaxHealthAttribute());

    return (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
}

void AMukCheonWangCharacter::Multicast_PlayAuraEffect_Implementation(UNiagaraSystem* InAuraEffect, TSubclassOf<AActor> InFontlClass, float InAuraRate)
{
    
    if (InAuraEffect)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            InAuraEffect,
            GetRootComponent(),
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::KeepRelativeOffset,
            true
        );

        if (NiagaraComp)
        {
            FTimerHandle DestroyHandle;
            FTimerDelegate DestroyDelegate = FTimerDelegate::CreateLambda([NiagaraComp]()
            {
                if (NiagaraComp)
                {
                    NiagaraComp->DestroyComponent();
                }
            });

            GetWorld()->GetTimerManager().SetTimer(
                DestroyHandle,
                DestroyDelegate,
                InAuraRate,
                false
            );
        }
    }
    
    if (InFontlClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        AActor* AuraDecal = GetWorld()->SpawnActor<AActor>(
            InFontlClass,
            GetActorLocation(),
            FRotator::ZeroRotator,
            SpawnParams
        );

        if (AuraDecal)
        {
            AuraDecal->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}
