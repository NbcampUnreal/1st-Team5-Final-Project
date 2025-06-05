#include "EnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "GenericTeamAgentInterface.h"
#include "NavigationInvokerComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	AbilitySystemComponent = CreateDefaultSubobject<UPRAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UPRAttributeSet>("AttributeSet");

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));
	StimuliSourceComponent->bAutoRegister = true;
	StimuliSourceComponent->RegisterForSense(UAISense_Hearing::StaticClass());
	
	PawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
}

void AEnemyCharacter::ToggleWeaponCollision(bool bEnable)
{
	if (Weapon)
	{
		Weapon->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (StimuliSourceComponent)
	{
		StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimuliSourceComponent->RegisterWithPerceptionSystem();
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (AttributeSet)
	{
		AbilitySystemComponent->AddAttributeSetSubobject(Cast<UPRAttributeSet>(AttributeSet));
	}

	BindToTagChange();

	Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoInitialized();
}

void AEnemyCharacter::AddCharacterAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		HasAuthority(), AbilitySystemComponent != nullptr;
		return;
	}

	for (TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbilities)
	{
		if (StartupAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}
	}
}

void AEnemyCharacter::Multicast_PlayAttackSound_Implementation(USoundBase* Sound)
{
	if (Sound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
	this,
	Sound,
	GetActorLocation(),
	FRotator::ZeroRotator,
	1.0f, 
	1.0f, 
	0.0f, 
	AttackAttenuation ,
	nullptr
	);
	}
}

void AEnemyCharacter::Die(FVector HitDirection)
{
	Super::Die(HitDirection);

	bIsDead = true;

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->CancelAllAbilities();
	}

	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* Brain = AICon->GetBrainComponent())
		{
			Brain->StopLogic(TEXT("Die"));
		}

		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsDead"), true);
			BB->ClearValue("TargetActor");
			BB->SetValueAsBool("bPlayerDetect", false);
			BB->SetValueAsBool("bIsAttacking", false);
		}
		AICon->UnPossess();
	}

	if (!bIsClone && ContainerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(
			ContainerClass,
			GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams
		);
	}
	SetLifeSpan(5.0f);
	
}

UAnimMontage* AEnemyCharacter::GetRandomAttackMontage() const
{
	if (AttackMontages.Num() == 0) return nullptr;

	int32 Index = FMath::RandRange(0, AttackMontages.Num() - 1);
	return AttackMontages[Index];
}

UAnimMontage* AEnemyCharacter::GetRandomDetectMontage() const
{
	if (DetectMontages.Num() == 0) return nullptr;

	int32 Index = FMath::RandRange(0, DetectMontages.Num() - 1);
	return DetectMontages[Index];
}

bool AEnemyCharacter::IsDead() const
{
	return bIsDead;
}

void AEnemyCharacter::PlayGradualSound(USoundBase* InSound, float MaxDistance, float Volume, float Loudness)
{
	if (!InSound) return;
	
	if (HasAuthority())
	{
		Multicast_PlayGradualSound(InSound, MaxDistance, Volume, Loudness);
	}
}

void AEnemyCharacter::Multicast_PlayGradualSound_Implementation(USoundBase* InSound, float MaxDistance, float Volume, float Loudness)
{
	if (!InSound) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector SoundLocation = GetActorLocation();
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (APawn* PlayerPawn = PC->GetPawn())
		{
			float Distance = FVector::Dist(SoundLocation, PlayerPawn->GetActorLocation());
			if (Distance <= MaxDistance)
			{
				float VolumeScale = FMath::Clamp(1.f - (Distance / MaxDistance), 0.f, 1.f) * Volume;
				UGameplayStatics::PlaySoundAtLocation(World, InSound, SoundLocation, VolumeScale);
			}
		}
	}

	MakeNoise(Loudness, this, SoundLocation, MaxDistance);
}

void AEnemyCharacter::PlayDetectMontage(UAnimMontage* Montage)
{
	if (!Montage || !GetMesh() || !GetMesh()->GetAnimInstance()) return;

	if (HasAuthority())
	{
		Multicast_PlayDetectMontage(Montage);
	}
}

void AEnemyCharacter::Multicast_PlayDetectMontage_Implementation(UAnimMontage* Montage)
{
	if (!Montage || !GetMesh()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Montage);
	}
}
