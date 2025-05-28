// PayRockGames

#include "WaterWave.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "PayRock/Character/PRCharacter.h"


AWaterWave::AWaterWave()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetBoxExtent(FVector(100.f, 300.f, 100.f));
	RootComponent = CollisionBox;

	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffect->SetupAttachment(RootComponent);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWaterWave::OnOverlapBegin);
}

void AWaterWave::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(Lifetime);
	
	MoveDirection = GetActorForwardVector().GetSafeNormal();
}

void AWaterWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DeltaMove = MoveDirection * MoveSpeed * DeltaTime;
	AddActorWorldOffset(DeltaMove, true);
}

void AWaterWave::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										 const FHitResult& SweepResult)
{
	if (APRCharacter* Character = Cast<APRCharacter>(OtherActor))
	{
		FVector KnockbackDir = MoveDirection;
		KnockbackDir.Z = 0.5f;
		KnockbackDir.Normalize();

		Character->LaunchCharacter(KnockbackDir * KnockbackStrength, true, true);
	}
}
