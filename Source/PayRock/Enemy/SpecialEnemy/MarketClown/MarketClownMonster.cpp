#include "MarketClownMonster.h"
#include "MarketClownMonsterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

AMarketClownMonster::AMarketClownMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	MaskMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaskMesh"));
	MaskMesh->SetupAttachment(GetMesh(), HeadSocketName);
	MaskMesh->SetRelativeLocation(FVector::ZeroVector);
	MaskMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(Weapon, CollisionSocketName);

	WeaponCollision->InitSphereRadius(30.0f);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponCollision->SetGenerateOverlapEvents(true);
	
	CurrentMask = ETalMaskType::Yangban;
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMarketClownMonsterController::StaticClass();
}

void AMarketClownMonster::BeginPlay()
{
	Super::BeginPlay();

	if (SplitLevel == 0)
	{
		MaxSplitCount = FMath::RandRange(2, 4);
	}
	
	AttackMontages.SetNum(4);
	ApplyMaskBehavior(CurrentMask);
}

void AMarketClownMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMarketClownMonster::ApplyMaskBehavior(ETalMaskType Mask)
{
	CurrentMask = Mask;

	switch (Mask)
	{
	case ETalMaskType::Yangban:
		GetCharacterMovement()->MaxWalkSpeed = YangbanSpeed;
		if (MaskMesh_Yangban) MaskMesh->SetSkeletalMesh(MaskMesh_Yangban);
		break;
	case ETalMaskType::Imae:
		GetCharacterMovement()->MaxWalkSpeed = ImaeSpeed;
		if (MaskMesh_Imae) MaskMesh->SetSkeletalMesh(MaskMesh_Imae);
		break;
	case ETalMaskType::Baekjeong:
		GetCharacterMovement()->MaxWalkSpeed = BaekjeSpeed;
		if (MaskMesh_Baekjeong) MaskMesh->SetSkeletalMesh(MaskMesh_Baekjeong);
		break;
	case ETalMaskType::Bune:
		GetCharacterMovement()->MaxWalkSpeed = BuneSpeed;
		if (MaskMesh_Bune) MaskMesh->SetSkeletalMesh(MaskMesh_Bune);
		break;
	default:
		break;
	}
}

UAnimMontage* AMarketClownMonster::GetCurrentMaskAttackMontage() const
{
	if (!AttackMontages.IsValidIndex(3)) return nullptr;

	switch (CurrentMask)
	{
	case ETalMaskType::Yangban: return AttackMontages[0];
	case ETalMaskType::Imae: return AttackMontages[1];
	case ETalMaskType::Baekjeong: return AttackMontages[2];
	case ETalMaskType::Bune: return AttackMontages[3];
	default: return AttackMontages[0];
	}
}

UAnimMontage* AMarketClownMonster::GetRoarMontage() const
{
	return RoarMontage;
}

AActor* AMarketClownMonster::GetBlackboardTarget() const
{
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			return Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
		}
	}
	return nullptr;
}

void AMarketClownMonster::SplitOnDeath()
{
	if (!GetWorld()) return;

	TArray<ETalMaskType> MaskPool = {
		ETalMaskType::Yangban,
		ETalMaskType::Imae,
		ETalMaskType::Baekjeong,
		ETalMaskType::Bune
	};

	FVector Origin = GetActorLocation();
	FVector RightOffset = GetActorRightVector() * 150.f;
	FVector LeftSpawn = Origin - RightOffset;
	FVector RightSpawn = Origin + RightOffset;

	FRotator SpawnRotation = GetActorRotation();
	TSubclassOf<AMarketClownMonster> CloneClass = GetClass();

	for (int i = 0; i < 2; ++i)
	{
		FVector SpawnLoc = (i == 0) ? LeftSpawn : RightSpawn;
		ETalMaskType RandomMask = MaskPool[FMath::RandRange(0, MaskPool.Num() - 1)];
		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLoc);

		AMarketClownMonster* Clone = GetWorld()->SpawnActorDeferred<AMarketClownMonster>(
			CloneClass,
			SpawnTransform,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);

		if (Clone)
		{
			Clone->ApplyMaskBehavior(RandomMask);
			Clone->InitAbilityActorInfo();
			Clone->BindToTagChange();
			Clone->AddCharacterAbilities();
			
			Clone->ApplySplitLevelAttributes(SplitLevel + 1);

			UGameplayStatics::FinishSpawningActor(Clone, SpawnTransform);

			FVector LaunchDir = (i == 0) ? -GetActorRightVector() : GetActorRightVector();
			LaunchDir.Z = 0.4f;
			Clone->LaunchCharacter(LaunchDir.GetSafeNormal() * 400.f, true, true);

			Clone->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Clone->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

			if (AMarketClownMonsterController* CloneController = Cast<AMarketClownMonsterController>(Clone->GetController()))
			{
				if (CloneController->GetBlackboardComponent())
				{
					CloneController->RunBehaviorTree(CloneController->GetBehaviorTree());
				}

				if (UBlackboardComponent* BB = CloneController->GetBlackboardComponent())
				{
					if (AActor* Target = GetBlackboardTarget())
					{
						BB->SetValueAsObject("TargetActor", Target);
						BB->SetValueAsBool("bPlayerDetect", true);
					}
					BB->SetValueAsBool("bIsDead", false);
					BB->SetValueAsBool("bIsAttacking", false);
					BB->SetValueAsBool("bIsBusy", false);
				}
			}
		}
	}

	Destroy();
}

void AMarketClownMonster::ApplySplitLevelAttributes(int32 InLevel)
{
	SplitLevel = InLevel;

	const float Scale = FMath::Clamp(1.f - (SplitLevel * 0.2f), 0.3f, 1.0f);
	const float InitialStrength = 10.0f;
	const float NewStrength = InitialStrength * Scale;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && StrengthOverrideEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(StrengthOverrideEffect, 1.f, ASC->MakeEffectContext());

		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Strength")),
				NewStrength
			);

			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
}

void AMarketClownMonster::Die()
{
	if (bIsDead) return;
	bIsDead = true;

	if (SplitLevel < MaxSplitCount)
	{
		SplitOnDeath();
		Super::Die();
		return;
	}

	if (UAnimMontage* DeathMontage = GetDeathMontage())
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([this](UAnimMontage*, bool)
			{
				Super::Die();
				Destroy();
			});
			Anim->Montage_Play(DeathMontage);
			Anim->Montage_SetEndDelegate(EndDelegate, DeathMontage);
			return;
		}
	}

	Super::Die();
	Destroy();
}
