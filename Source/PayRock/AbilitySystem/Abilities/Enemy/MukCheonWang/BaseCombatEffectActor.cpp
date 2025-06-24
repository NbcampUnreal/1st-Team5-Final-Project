#include "BaseCombatEffectActor.h"
#include "NiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "Perception/AISense_Damage.h"

ABaseCombatEffectActor::ABaseCombatEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ABaseCombatEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(DefaultLifeSpan);

	if (HasAuthority())
	{
		Multicast_PlayVFX();
	}

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseCombatEffectActor::OnEffectOverlap);
	}
}

void ABaseCombatEffectActor::Multicast_PlayVFX_Implementation()
{
	if (VFX && GetNetMode() != NM_DedicatedServer)
	{
		VFX->Activate(true);
	}
}

void ABaseCombatEffectActor::InitializeEffectSource(UGameplayAbility* InAbility)
{
	if (InAbility)
	{
		InstigatorAbility = Cast<UBaseDamageGameplayAbility>(InAbility); 
	}
}

void ABaseCombatEffectActor::OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                             bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	ApplyDamageEffect(OtherActor);
}

void ABaseCombatEffectActor::ApplyDamageEffect(AActor* TargetActor)
{
	if (!InstigatorAbility || !TargetActor || !HasAuthority()) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		InstigatorAbility->GetAvatarActorFromActorInfo());

	if (!TargetASC || !SourceASC || !DamageEffectClass) return;

	
	bool bIsMonsterToMonster = InstigatorAbility->GetAvatarActorFromActorInfo()->IsA(AEnemyCharacter::StaticClass()) &&
							   TargetActor->IsA(AEnemyCharacter::StaticClass());

	float FinalDamage = DamageAmount;
	if (bIsMonsterToMonster)
	{
		FinalDamage *= 0.5f;
	}

	
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, SourceASC->MakeEffectContext());
	if (SpecHandle.IsValid())
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, FinalDamage);
		SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

		
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			TargetActor,
			InstigatorAbility->GetAvatarActorFromActorInfo(),
			FinalDamage,
			TargetActor->GetActorLocation(),
			InstigatorAbility->GetAvatarActorFromActorInfo()->GetActorLocation()
		);
	}
}
