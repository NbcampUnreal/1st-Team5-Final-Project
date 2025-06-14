#include "GA_ChargeAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/SpecialEnemy/JangSanTiger/JasnSanTiger.h"

UGA_ChargeAttack::UGA_ChargeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ChargeAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	Tiger = Cast<AJasnSanTiger>(ActorInfo->AvatarActor.Get());
	if (!Tiger) return;

	if (AAIController* AICon = Cast<AAIController>(Tiger->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", true);
			AICon->StopMovement();
		}
	}
	
}

void UGA_ChargeAttack::OnChargeStart()
{
	if (!Tiger) return;

	// if (Tiger->ChargeCollision)
	// {
	// 	Tiger->ChargeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// 	Tiger->ChargeCollision->OnComponentBeginOverlap.AddDynamic(this, &UGA_ChargeAttack::OnChargeHit);
	// 	UE_LOG(LogTemp, Warning, TEXT("콜리전 활성화"));
	// }
	
	const FVector Forward = Tiger->GetActorForwardVector();
	GetWorld()->GetTimerManager().SetTimer(SweepTimerHandle, this, &UGA_ChargeAttack::DoChargeSweep, 0.15f, true);
	Tiger->LaunchCharacter(Forward * ChargeSpeed, true, true);

	

}


void UGA_ChargeAttack::EndCharge()
{
	if (!Tiger) return;

	HitActors.Empty();
	GetWorld()->GetTimerManager().ClearTimer(SweepTimerHandle);
	
	// if (Tiger->ChargeCollision)
	// {
	// 	bHit = false;
	// 	Tiger->ChargeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 	Tiger->ChargeCollision->OnComponentBeginOverlap.RemoveAll(this);
	// }

	if (AAIController* AICon = Cast<AAIController>(Tiger->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", false);
			BB->SetValueAsBool("bInAttackRange", false);
		}
	}

}

void UGA_ChargeAttack::DoChargeSweep()
{
	if (!Tiger) return;

	const FVector Start = Tiger->GetActorLocation();
	const FVector Forward = Tiger->GetActorForwardVector();
	const FVector End = Start + Forward * 150.f;

	const FVector Location = (Start + End) * 0.5f;
	const FQuat Rotation = Tiger->GetActorQuat(); 

	FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Tiger);

	TArray<FHitResult> OutHits;
	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		Start,
		End,
		Rotation,
		ECC_Pawn,
		Box,
		Params
	);

	// const FColor BoxColor = bHit ? FColor::Red : FColor::Green;
	// DrawDebugBox(GetWorld(), Location, BoxExtent, Rotation, BoxColor, false, 1.0f);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);
				CauseDamage(HitActor, Hit);
			}
		}
	}
}