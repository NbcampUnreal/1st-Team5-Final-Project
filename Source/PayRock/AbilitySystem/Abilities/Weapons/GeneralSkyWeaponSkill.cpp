// PayRockGames

#include "GeneralSkyWeaponSkill.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PayRock/Character/PRCharacter.h"
#include "Engine/OverlapResult.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"

void UGeneralSkyWeaponSkill::OnEventReceived(FGameplayEventData Payload)
{
	if (APRCharacter* Character = Cast<APRCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->LaunchCharacter(FVector(0.f, 0.f, LaunchSpeed), true, true);
		Character->LandedDelegate.AddUniqueDynamic(this, &UGeneralSkyWeaponSkill::OnLanded);
		/*if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
		{
			Movement->StopMovementImmediately();
			Movement->DisableMovement();
		}*/
	}
}

void UGeneralSkyWeaponSkill::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UniqueActors.Empty();

	/*if (APRCharacter* Character = Cast<APRCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
		{
			Movement->SetMovementMode(MOVE_Walking);
		}
	}*/
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGeneralSkyWeaponSkill::OnLanded(const FHitResult& Hit)
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->PlayAnimMontage(LandMontage);
		Character->LandedDelegate.RemoveAll(this);
	}
	
	FVector Origin = GetAvatarActorFromActorInfo()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());	
	QueryParams.bTraceIntoSubComponents = false;

	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECC_Pawn);
	
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		Origin,
		FQuat::Identity,
		CollisionParams,
		Sphere,
		QueryParams
	);
	
	if (bHit)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC) return;
		
		for (const FOverlapResult& Result : Overlaps)
		{
			if (AActor* Actor = Result.GetActor())
			{
				if (UniqueActors.Contains(Actor)) continue;

				UniqueActors.Add(Actor);
				
				if (UAbilitySystemComponent* TargetASC =
					UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
				{
					if (UPRAbilitySystemComponent* PRASC = Cast<UPRAbilitySystemComponent>(TargetASC))
					{
						PRASC->StoredHitDirection = (Actor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
					}
					FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
					ContextHandle.AddOrigin(Actor->GetActorLocation());
					FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
						KnockbackEffectClass, GetAbilityLevel(), ContextHandle);
					SourceASC->ApplyGameplayEffectSpecToTarget(
						*SpecHandle.Data.Get(),
						TargetASC
					);
				}

				if (GetAvatarActorFromActorInfo()->HasAuthority())
				{
					CauseDamage(Actor, FHitResult());
				}
			}
		}
	}
	
	K2_EndAbility();
}
