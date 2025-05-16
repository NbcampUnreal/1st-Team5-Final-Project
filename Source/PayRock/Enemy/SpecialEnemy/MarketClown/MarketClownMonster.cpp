#include "MarketClownMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

AMarketClownMonster::AMarketClownMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentMask = EMaskType::Yangban;
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

void AMarketClownMonster::ApplyMaskBehavior(EMaskType Mask)
{
	CurrentMask = Mask;

	switch (Mask)
	{
	case EMaskType::Yangban:
		GetCharacterMovement()->MaxWalkSpeed = YangbanSpeed;
		break;
	case EMaskType::Imae:
		GetCharacterMovement()->MaxWalkSpeed = ImaeSpeed;
		break;
	case EMaskType::Baekjeong:
		GetCharacterMovement()->MaxWalkSpeed = BaekjeSpeed;
		break;
	case EMaskType::Bune:
		GetCharacterMovement()->MaxWalkSpeed = BuneSpeed;
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

	TArray<EMaskType> MaskPool = {
		EMaskType::Yangban,
		EMaskType::Imae,
		EMaskType::Baekjeong,
		EMaskType::Bune
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
		EMaskType RandomMask = MaskPool[FMath::RandRange(0, MaskPool.Num() - 1)];

		AMarketClownMonster* Clone = GetWorld()->SpawnActor<AMarketClownMonster>(CloneClass, SpawnLoc, SpawnRotation);
		if (Clone)
		{
			Clone->MaskAttackMontages = MaskAttackMontages;
			Clone->ApplyMaskBehavior(RandomMask);
			Clone->InitSplitLevel(SplitLevel + 1);
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
