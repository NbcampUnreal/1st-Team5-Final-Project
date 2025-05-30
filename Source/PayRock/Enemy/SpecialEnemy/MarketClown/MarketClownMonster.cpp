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
	

	bIsDead = false;
	
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
		ETalMaskType::Yangban, ETalMaskType::Imae,
		ETalMaskType::Baekjeong, ETalMaskType::Bune
	};

	FVector Origin = GetActorLocation();
	FVector RightOffset = GetActorRightVector() * 150.f;
	FVector LeftSpawn = Origin - RightOffset;
	FVector RightSpawn = Origin + RightOffset;
	FRotator SpawnRotation = GetActorRotation();
	TSubclassOf<AMarketClownMonster> CloneClass = GetClass();

	UClass* AnimClass = GetMesh()->GetAnimInstance() ? GetMesh()->GetAnimInstance()->GetClass() : nullptr;

	for (int i = 0; i < 2; ++i)
	{
		FVector SpawnLoc = (i == 0) ? LeftSpawn : RightSpawn;
		ETalMaskType RandomMask = MaskPool[FMath::RandRange(0, MaskPool.Num() - 1)];
		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLoc);

		AMarketClownMonster* Clone = GetWorld()->SpawnActorDeferred<AMarketClownMonster>(
			CloneClass, SpawnTransform, nullptr, nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

		if (Clone)
		{
			Clone->bIsClone = true;
			Clone->SplitLevel = SplitLevel + 1;
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				Clone->GetMesh()->SetAnimInstanceClass(AnimInstance->GetClass());
			}
			Clone->ApplyMaskBehavior(RandomMask);
			Clone->InitAbilityActorInfo();
			Clone->AddCharacterAbilities();
			Clone->ApplySplitLevelAttributes(Clone->SplitLevel);
			

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
					UBlackboardComponent* BB = CloneController->GetBlackboardComponent();
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
	
	const float ScaledStrength = FMath::Clamp(10.f - (SplitLevel * 2.5f), 1.f, 10.f);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC) return;

	if (GE_ClonePrimaryInit)
	{
		FGameplayEffectSpecHandle PrimarySpec = ASC->MakeOutgoingSpec(GE_ClonePrimaryInit, 1.f, ASC->MakeEffectContext());
		if (PrimarySpec.IsValid())
		{
			PrimarySpec.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Strength")),
				ScaledStrength
			);

			ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpec.Data);
		}
	}
	
	if (GE_CloneSecondaryInit)
	{
		FGameplayEffectSpecHandle SecondarySpec = ASC->MakeOutgoingSpec(GE_CloneSecondaryInit, 1.f, ASC->MakeEffectContext());
		if (SecondarySpec.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpec.Data);
		}
	}
	
	if (GE_CloneVitalInit)
	{
		FGameplayEffectSpecHandle VitalSpec = ASC->MakeOutgoingSpec(GE_CloneVitalInit, 1.f, ASC->MakeEffectContext());
		if (VitalSpec.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*VitalSpec.Data);
		}
	}
}


void AMarketClownMonster::Die(FVector HitDirection)
{
	if (bIsDead) return;
	bIsDead = true;

	if (SplitLevel < MaxSplitCount)
	{
		SplitOnDeath();
		Super::Die(HitDirection);
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

	Super::Die(HitDirection);
	Destroy();
}
