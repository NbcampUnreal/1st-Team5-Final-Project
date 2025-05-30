// PayRockGames


#include "MukCheonWangCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Enemy/EBossPhase.h"


AMukCheonWangCharacter::AMukCheonWangCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 100000.f;
    SightConfig->LoseSightRadius = 100200.f;
    SightConfig->PeripheralVisionAngleDegrees = 360.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 100000.f;
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
                }
                if (NewPhase == EBossPhase::Phase3)
                {
                    BB->SetValueAsBool(TEXT("bIsSpecialPattern2"), true);
                }
            }
        }
    }
}

void AMukCheonWangCharacter::ToggleVisibleSkeletalMesh(bool isActive)
{
    ChairMesh->SetVisibility(isActive);
}

void AMukCheonWangCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor || !Actor->IsA(APRCharacter::StaticClass())) return;

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
            BB->SetValueAsObject(TEXT("TargetActor"), Actor);
            BB->SetValueAsBool(TEXT("bPlayerDetect"), true);
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
        if (Actor && Actor->IsA(APRCharacter::StaticClass()))
        {
            DetectedActors.Add(Actor);
        }
    }

    if (DetectedActors.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, DetectedActors.Num() - 1);
        AActor* ChosenTarget = DetectedActors[Index];

        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
            {
                BB->SetValueAsObject(TEXT("TargetActor"), ChosenTarget);
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