#include "ThrowingWeaponBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "Perception/AISense_Damage.h"

AThrowingWeaponBase::AThrowingWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(8.f);
	Collision->SetCollisionProfileName("Projectile");
	Collision->SetupAttachment(RootComponent);
	Collision->SetGenerateOverlapEvents(true);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AThrowingWeaponBase::OnBeginOverlap);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	TrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(WeaponMesh); 
	TrailEffect->SetAutoActivate(false); 
	TrailEffect->SetUsingAbsoluteRotation(true); 
}


void AThrowingWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	
	FVector Forward = GetActorForwardVector();
	FVector LaunchDir = Forward.GetSafeNormal();
	ProjectileMovement->Velocity = LaunchDir * TravelSpeed;
	
	if (HasAuthority())
	{
		MulticastActivateTrail();
	}
}


void AThrowingWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Travelled = FVector::Dist(InitialLocation, GetActorLocation());
	if (Travelled >= MaxTravelDistance)
	{
		Destroy();
	}
}

void AThrowingWeaponBase::LaunchWeapon(const FVector& LaunchVelocity)
{
	ProjectileMovement->Velocity = LaunchVelocity;
}

void AThrowingWeaponBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										 bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor || OtherActor == GetOwner()) return;

	if (TrailEffect && TrailEffect->IsActive())
	{
		TrailEffect->Deactivate();
	}

	ApplyDamageEffect(OtherActor);
	Destroy();
}


void AThrowingWeaponBase::ApplyDamageEffect(AActor* TargetActor)
{
	
	if (!InstigatorAbility || !TargetActor || !HasAuthority()) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	UAbilitySystemComponent* Source = SourceASC;
	if (!Source && InstigatorAbility)
	{
		Source = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			InstigatorAbility->GetAvatarActorFromActorInfo());
	}
	
	if (!TargetASC || !SourceASC || !DamageEffectClass) return;


	float FinalDamageValue = Damage.GetValueAtLevel(1.0f);

	if (InstigatorAbility->GetAvatarActorFromActorInfo()->IsA(AEnemyCharacter::StaticClass()) &&
		TargetActor->IsA(AEnemyCharacter::StaticClass()))
	{
		FinalDamageValue *= 0.5f;
	}


	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, SourceASC->MakeEffectContext());
	if (SpecHandle.IsValid())
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, FinalDamageValue);
		SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			TargetActor,
			InstigatorAbility->GetAvatarActorFromActorInfo(),
			FinalDamageValue,
			TargetActor->GetActorLocation(),
			InstigatorAbility->GetAvatarActorFromActorInfo()->GetActorLocation()
		);
	}
}

void AThrowingWeaponBase::StartTrail()
{
	if (TrailEffect && TrailEffectTemplate)
	{
		TrailEffect->SetAsset(TrailEffectTemplate);
		TrailEffect->Activate(true);
	}
}

void AThrowingWeaponBase::MulticastActivateTrail_Implementation()
{
	StartTrail();
}