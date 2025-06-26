// PayRockGames

#include "DoggebiSpinActor.h"
#include "Components/BoxComponent.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"

ADoggebiSpinActor::ADoggebiSpinActor()
	: AttackSpeed(720.f), IdleSpeed(120.f), bShouldAttack(false), bCanAttack(true)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
}

void ADoggebiSpinActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);

	if (IsValid(BoxComp))
	{
		BoxComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADoggebiSpinActor::OnOverlap);
		/*BaseBoxExtent = BoxComp->GetScaledBoxExtent();
		BaseBoxOffset = BoxComp->GetRelativeLocation().X;
	}

	if (IsValid(StaticMeshComp))
	{
		BaseMeshScale = StaticMeshComp->GetRelativeScale3D();*/
	}
}

void ADoggebiSpinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaYaw = (bShouldAttack ? AttackSpeed : IdleSpeed) * DeltaTime;
	AddActorLocalRotation(FRotator(0.f, DeltaYaw, 0.f));
	TotalRotationYaw += DeltaYaw;
	if (TotalRotationYaw >= 720.0f) {
		TotalRotationYaw = 0.f;
		OnRotationComplete();
	}
    /*
	// Interpolate box extent increase
	float TargetExtension = bShouldAttack ? AttackReach : 0.0f;
	CurrentExtension = FMath::FInterpTo(CurrentExtension, TargetExtension, DeltaTime, BoxExtensionSpeed);
    
	// Update box
	FVector CurrentExtent = BaseBoxExtent;
	CurrentExtent.X += CurrentExtension;
	BoxComp->SetBoxExtent(CurrentExtent);
    
	float ExtensionOffset = CurrentExtension * 0.5f;
	FVector NewLocation = FVector(BaseBoxOffset + ExtensionOffset, 0, 0);
	BoxComp->SetRelativeLocation(NewLocation);
    
	// Scale the mesh based on the extension
	float ScaleFactor = 1.f;
	if (AttackReach > 0.f)
	{
		ScaleFactor = 1.f + (CurrentExtension / AttackReach) * MeshScaleMultiplier;
	}
	FVector NewScale = BaseMeshScale;
	NewScale.X *= ScaleFactor;
	StaticMeshComp->SetRelativeScale3D(NewScale);
	*/
}

void ADoggebiSpinActor::StartAttack()
{
	if (bCanAttack && IsValid(GetWorld()) && !GetWorld()->bIsTearingDown)
	{
		bShouldAttack = true;
		bCanAttack = false;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ADoggebiSpinActor::SetCanAttack, Cooldown);
	}
}


void ADoggebiSpinActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bShouldAttack || !IsValid(OtherActor) || OtherActor == SourceActor) return;

	if (HasAuthority() && IsValid(SourceDamageAbility))
	{
		if (AlreadyHitActors.Contains(OtherActor)) return;
		AlreadyHitActors.Add(OtherActor);
		SourceDamageAbility->CauseDamage(OtherActor, SweepResult);
	}
}

void ADoggebiSpinActor::OnRotationComplete()
{
	bShouldAttack = false;
	AlreadyHitActors.Empty();
}

void ADoggebiSpinActor::SetCanAttack()
{
	bCanAttack = true;
}

