#include "MarketClownMonster.h"
#include "MarketClownMonsterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

AMarketClownMonster::AMarketClownMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	MaskMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaskMesh"));
	MaskMesh->SetupAttachment(GetMesh(), TEXT("Head"));
	MaskMesh->SetRelativeLocation(FVector::ZeroVector);
	MaskMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CurrentMask = ETalMaskType::Yangban;
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMarketClownMonsterController::StaticClass();
}

void AMarketClownMonster::BeginPlay()
{
	Super::BeginPlay();
	
	MaskAttackMontages.SetNum(4);
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
	int32 Index = static_cast<int32>(CurrentMask);
	if (MaskAttackMontages.IsValidIndex(Index))
	{
		return MaskAttackMontages[Index];
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
		
		AMarketClownMonster* Clone = GetWorld()->SpawnActorDeferred<AMarketClownMonster>(CloneClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (Clone)
		{
			Clone->MaskAttackMontages = MaskAttackMontages;
			Clone->ApplyMaskBehavior(RandomMask);
			Clone->InitSplitLevel(SplitLevel + 1);
			
			UGameplayStatics::FinishSpawningActor(Clone, SpawnTransform);

			// ✅ 콜리전 및 이동 가능 상태로 복구
			Clone->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Clone->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

			// ✅ 튀어나오는 느낌의 런치
			FVector LaunchDir = (i == 0) ? -GetActorRightVector() : GetActorRightVector();
			LaunchDir.Z = 0.4f;
			Clone->LaunchCharacter(LaunchDir.GetSafeNormal() * 400.f, true, true);
		}
	}

	Destroy();
}

void AMarketClownMonster::InitSplitLevel(int32 InLevel)
{
	SplitLevel = InLevel;
	const float Scale = FMath::Clamp(1.f - (SplitLevel * 0.2f), 0.3f, 1.0f);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (UPRAttributeSet* AS = Cast<UPRAttributeSet>(GetAttributeSet()))
		{
			const float NewMaxHealth = AS->GetMaxHealth() * Scale;
			const float NewAttackSpeed = AS->GetAttackSpeed() * Scale;

			ASC->SetNumericAttributeBase(UPRAttributeSet::GetMaxHealthAttribute(), NewMaxHealth);
			ASC->SetNumericAttributeBase(UPRAttributeSet::GetHealthAttribute(), NewMaxHealth);
			ASC->SetNumericAttributeBase(UPRAttributeSet::GetAttackSpeedAttribute(), NewAttackSpeed);
		}
	}
}

void AMarketClownMonster::Die()
{
	Super::Die();

	if (SplitLevel < 2)
	{
		SplitOnDeath();
	}
	else
	{
		Destroy();
	}
}
