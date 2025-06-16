// PayRockGames

#include "SkeletonMonster.h"
#include "SkeletonMonsterController.h"

ASkeletonMonster::ASkeletonMonster()
{
	AIControllerClass = ASkeletonMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CharacterType = ECharacterType::Skeleton;

	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(Weapon, CollisionSocketName);
	WeaponCollision->InitSphereRadius(50.f); 
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponCollision->SetGenerateOverlapEvents(true);
}

void ASkeletonMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

