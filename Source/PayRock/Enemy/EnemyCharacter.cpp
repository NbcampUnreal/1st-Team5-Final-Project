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
#include "PayRock/Character/Buff/BuffComponent.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/GameSystem/PRAdvancedGameInstance.h"
#include "PayRock/GameSystem/PRGameState.h"
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
	BuffComponent = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));

	bReplicates = true;
	bAlwaysRelevant = true;

	GetMesh()->SetIsReplicated(true);
	GetMesh()->SetOnlyOwnerSee(false);
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

	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		SavedAnimClass = GetMesh()->GetAnimInstance() ? GetMesh()->GetAnimInstance()->GetClass() : nullptr;
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
void AEnemyCharacter::BindToTagChange()
{
	Super::BindToTagChange();
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Status_Debuff_Frozen).AddUObject(
		BuffComponent, &UBuffComponent::OnFrozenTagChange);
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Status_Debuff_Shocked).AddUObject(
		BuffComponent, &UBuffComponent::OnShockedTagChange);
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

void AEnemyCharacter::Client_NotifyQuestKill_Implementation(APlayerController* KillerPC)
{
	if (!KillerPC) return;

	if (!KillerPC->IsLocalController()) return;
	
	if (UPRAdvancedGameInstance* PRGI = Cast<UPRAdvancedGameInstance>(UGameplayStatics::GetGameInstance(KillerPC)))
	{
		const FString TargetName = PRGI->GetQuestManager()->GetCurrentQuest().TargetName;

		const FName CharacterTypeName = StaticEnum<ECharacterType>()->GetNameByValue((int64)CharacterType);
		FString CharacterTypeString = CharacterTypeName.ToString();
		FString ShortName;
		CharacterTypeString.Split(TEXT("::"), nullptr, &ShortName);

		UE_LOG(LogTemp, Warning, TEXT("[퀘스트] 몬스터를 죽인 컨트롤러: %s (IsLocal=%d)"),
			*KillerPC->GetName(),
			KillerPC->IsLocalController());
		
		if (TargetName == ShortName)
		{
			PRGI->GetQuestManager()->UpdateProgress();
			UE_LOG(LogTemp, Log, TEXT("[Client] 퀘스트 진행도 증가: %s"), *ShortName);
		}
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
			if (!BB || !BB->GetBlackboardAsset()) return;
			
			BB->SetValueAsBool(FName("bIsDead"), true);
			BB->ClearValue("TargetActor");
			BB->SetValueAsBool("bPlayerDetect", false);
			BB->SetValueAsBool("bIsAttacking", false);
		}
		AICon->UnPossess();
	}

	if (HasAuthority())
	{
		if (APRGameState* GS = GetWorld()->GetGameState<APRGameState>())
		{
			GS->AddDieMonsterCount();
		}
	}

	if (HasAuthority() && LastHitInstigator)
	{
		ACharacter* KillerPawn = LastHitInstigator->GetCharacter();
		APlayerController* KillerPC = Cast<APlayerController>(LastHitInstigator);

		FString InstigatorName = LastHitInstigator->GetName();
		FString IsLocalStr = LastHitInstigator->IsLocalController() ? TEXT("Local") : TEXT("Remote");
		UE_LOG(LogTemp, Warning, TEXT("[EnemyCharacter_Die] LastHitInstigator: %s (%s)"), *InstigatorName, *IsLocalStr);

		if (KillerPawn && KillerPC)
		{
			Client_NotifyQuestKill(KillerPC);  
			UE_LOG(LogTemp, Log, TEXT("[EnemyCharacter_Die]Client_NotifyQuestKill"));
		}
	}
	
	if (ContainerClass)
	{
		FVector Start = GetActorLocation() + FVector(0.f, 0.f, 100.f);
		FVector End = GetActorLocation() - FVector(0.f, 0.f, 500.f);   

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			FVector GroundLocation = Hit.ImpactPoint;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AActor* Spawned = GetWorld()->SpawnActor<AActor>(
				ContainerClass,
				GroundLocation,
				FRotator::ZeroRotator,
				SpawnParams
			);
		}
	}

	Destroy();
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

void AEnemyCharacter::DisableAnimInstance()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->bPauseAnims = true;
		MeshComp->bNoSkeletonUpdate = true;
		
		if (MeshComp->GetAnimInstance())
		{
			SavedAnimClass = MeshComp->GetAnimInstance()->GetClass();
			MeshComp->SetAnimInstanceClass(nullptr);
		}
	}
}

void AEnemyCharacter::RestoreAnimInstance()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->bPauseAnims = false;
		MeshComp->bNoSkeletonUpdate = false;
		
		if (SavedAnimClass)
		{
			MeshComp->SetAnimInstanceClass(SavedAnimClass);
		}
	}
}