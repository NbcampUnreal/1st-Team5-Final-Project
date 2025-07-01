// PayRockGames

#include "BaseWeaponAbility.h"
#include "Components/CapsuleComponent.h"
#include "PayRock/Character/PRCharacter.h"

void UBaseWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	CurrentAttackType = EAttackType::NormalAttack;
	if (APRCharacter* Character = Cast<APRCharacter>(GetAvatarActorFromActorInfo()))
	{
		UpdateCurrentAttackType(Character);
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBaseWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AlreadyHitActors.Empty();
	if (APRCharacter* PlayerCharacter = Cast<APRCharacter>(GetAvatarActorFromActorInfo()))
	{
		/*UE_LOG(LogTemp, Warning, TEXT("[EndAbility] MOVE_Walking 복구 RPC 호출"));*/

		if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			PC->SetIgnoreMoveInput(false);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBaseWeaponAbility::PerformSweep()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor) || !AvatarActor->HasAuthority()) return;
	ABaseCharacter* Character = Cast<ABaseCharacter>(AvatarActor);
	if (!IsValid(Character)) return;

	// Set sweep size and distance
	const FVector Forward = AvatarActor->GetActorForwardVector();
	const FVector Start = AvatarActor->GetActorLocation() + Forward * SweepForwardOffset;
	// Modify distance for Jump and Dash attacks
	float SweepMultiplier = 1.f;
	switch (CurrentAttackType)
	{
	case EAttackType::DashAttack:
		SweepMultiplier = DashDistanceMultiplier;
		break;
	case EAttackType::JumpAttack:
		SweepMultiplier = JumpDistanceMultiplier;
		break;
	default: ;
	}
	const FVector End = Start + Forward * SweepDistance * SweepMultiplier;
	const FQuat Rotation = Forward.ToOrientationQuat(); 

	float SweepHeight = 0.f;
	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	SweepHeight = IsValid(Capsule) ? 2.f * Capsule->GetScaledCapsuleHalfHeight() : SweepWidth;

	FVector BoxExtent = FVector(10.f, SweepWidth, SweepHeight);
	FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	Params.bTraceIntoSubComponents = false;
	
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
	
	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !AlreadyHitActors.Contains(HitActor))
			{
				FVector TargetForward = HitActor->GetActorForwardVector().GetSafeNormal();
				FVector ToAttacker = (Character->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal();

				float Dot = FVector::DotProduct(TargetForward, ToAttacker);
				bool bIsBackAttack = Dot < -0.5f;
				
				AlreadyHitActors.Add(HitActor);
				CauseDamage(HitActor, Hit, bIsBackAttack);	
			}
		}
	}
}

void UBaseWeaponAbility::ResetHitActors()
{
	AlreadyHitActors.Empty();
}

void UBaseWeaponAbility::UpdateCurrentAttackType(APRCharacter* PlayerCharacter)
{
	if (PlayerCharacter->bIsInAir)
	{
		CurrentAttackType = EAttackType::JumpAttack;
		return;
	}

	if (PlayerCharacter->bIsSprinting)
	{
		CurrentAttackType = EAttackType::DashAttack;
	}
}
