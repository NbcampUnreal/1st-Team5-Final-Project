#include "EnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "EnemyController.h"
#include "GenericTeamAgentInterface.h"
#include "NavigationInvokerComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/GameSystem/PRGameState.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
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

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);
	SetNetUpdateFrequency(10.f);   // 1초에 최대 10회
	SetMinNetUpdateFrequency(.5f);    // 최소 5Hz
	SetNetCullDistanceSquared(FMath::Square(ActivationRadius * 1.1f));
	
	ActivationSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ActivationSphere"));
	ActivationSphere->SetupAttachment(RootComponent);
	ActivationSphere->SetSphereRadius(ActivationRadius);
	ActivationSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActivationSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivationSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	GetMesh()->SetIsReplicated(true);
	GetMesh()->SetOnlyOwnerSee(false);

	LODMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LODMeshComp"));
	LODMeshComp->SetupAttachment(RootComponent);
	LODMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LODMeshComp->SetCastShadow(false);
	LODMeshComp->SetVisibility(false);  // 처음엔 숨김
	
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
	ActivationSphere->OnComponentBeginOverlap.AddDynamic(
	this,
	&AEnemyCharacter::OnActivationOverlapBegin
	);
	ActivationSphere->OnComponentEndOverlap.AddDynamic(
		this,
		&AEnemyCharacter::OnActivationOverlapEnd
	);

	// 처음엔 비활성화 상태
	SetActivated(false);

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

	if (HasAuthority())
	{
		if (APRGameState* GS = GetWorld()->GetGameState<APRGameState>())
		{
			GS->DieMonsterCount++;
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
void AEnemyCharacter::Server_CheckPlayerProximity()
{
	if (!HasAuthority() || IsDead()) return;

	const float CheckDistance = 15000.f;
	bool bShouldBeActive = false;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC || !PC->GetPawn()) continue;

		const float Distance = FVector::Dist(GetActorLocation(), PC->GetPawn()->GetActorLocation());
		if (Distance <= CheckDistance)
		{
			bShouldBeActive = true;
			break;
		}
	}

	if (bShouldBeActive != !bIsServerFar)
	{
		bIsServerFar = !bShouldBeActive;

		// 1. 메쉬 및 충돌
		SetActorHiddenInGame(bIsServerFar);
		GetMesh()->SetComponentTickEnabled(!bIsServerFar);
		SetActorEnableCollision(!bIsServerFar);

		// 2. AI Logic 제어
		if (AAIController* AICon = Cast<AAIController>(GetController()))
		{
			if (UBrainComponent* Brain = AICon->GetBrainComponent())
			{
				if (bIsServerFar)
					Brain->StopLogic(TEXT("Far from player"));
				else
					Brain->StartLogic();
			}
			
			if (UAIPerceptionComponent* Perception = AICon->GetAIPerceptionComponent())
			{
				Perception->SetComponentTickEnabled(!bIsServerFar);
				Perception->SetActive(!bIsServerFar);
			}
		}

		// 3. BT 제어용 Blackboard 설정
		if (AAIController* AICon = Cast<AAIController>(GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool(FName("bIsFar"), bIsServerFar);
			}
		}
	}
}

bool AEnemyCharacter::IsPlayerCurrentlyDetected() const
{
	return bIsActivated;
}

void AEnemyCharacter::OnActivationOverlapBegin(
	UPrimitiveComponent* Overlapped, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Sweep)
{
	if (APawn* Pawn = Cast<APawn>(Other))
	{
		OverlappingPlayers.Add(Pawn);
		if (OverlappingPlayers.Num() == 1)
		{
			SetActivated(true);
		}
	}
}

void AEnemyCharacter::OnActivationOverlapEnd(
	UPrimitiveComponent* Overlapped, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APawn* Pawn = Cast<APawn>(Other))
	{
		OverlappingPlayers.Remove(Pawn);
		if (OverlappingPlayers.Num() == 0)
		{
			SetActivated(false);
		}
	}
}

void AEnemyCharacter::SetActivated(bool bNewActive)
{
	if (bIsActivated == bNewActive) return;
	bIsActivated = bNewActive;

	if (bIsActivated)
	{
		// --- AI Logic & Perception 켜기 ---
		if (AAIController* AICon = Cast<AAIController>(GetController()))
		{
			if (auto Brain = AICon->GetBrainComponent())            Brain->StartLogic();
			if (auto Perc  = AICon->GetAIPerceptionComponent())
			{
				Perc->SetActive(true);
				Perc->SetComponentTickEnabled(true);
			}
		}
		USkeletalMeshComponent* SkeletalMesh = GetMesh();
		SkeletalMesh->SetVisibility(true);
		SkeletalMesh->SetComponentTickEnabled(true);
		SkeletalMesh->GlobalAnimRateScale = 1.f;
		SkeletalMesh->bPauseAnims       = false;
		SkeletalMesh->bNoSkeletonUpdate = false;

		LODMeshComp->SetVisibility(false);
		// --- 콜리전 켜기 ---
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// --- 네트워크 Awake & 즉시 업데이트 ---
		SetNetDormancy(DORM_Awake);
		ForceNetUpdate();
	}
	else
	{
		// --- AI Logic & Perception 끄기 ---
		if (AAIController* AICon = Cast<AAIController>(GetController()))
		{
			if (auto Brain = AICon->GetBrainComponent())
				Brain->StopLogic(TEXT("Deactivated"));
			if (auto Perc = AICon->GetAIPerceptionComponent())
			{
				Perc->SetActive(false);
				Perc->SetComponentTickEnabled(false);
			}
		}
		// ─── Skeletal Mesh 애니메이션 완전 중지 ───
		USkeletalMeshComponent* SkeletalMesh = GetMesh();
		SkeletalMesh->GlobalAnimRateScale = 0.f;
		SkeletalMesh->bPauseAnims       = true;
		SkeletalMesh->bNoSkeletonUpdate = true;
		SkeletalMesh->SetComponentTickEnabled(false);
		SkeletalMesh->SetVisibility(false);

		// → 스태틱 메시 보이기
		LODMeshComp->SetVisibility(true);
		// ─── 콜리전 끄기 ───
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ─── 네트워크 Dormancy ───
		SetNetDormancy(DORM_DormantAll);
	}

	// 클라이언트에도 즉시 동기화하려면
	ForceNetUpdate();
}