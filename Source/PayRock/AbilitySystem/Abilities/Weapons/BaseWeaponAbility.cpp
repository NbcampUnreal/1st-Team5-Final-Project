// PayRockGames

#include "BaseWeaponAbility.h"
#include "Components/ShapeComponent.h"
#include "PayRock/Character/PRCharacter.h"

void UBaseWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	bHit = false;
	if (!CollisionComponents.IsEmpty()) return;
	if (ABaseCharacter* AvatarCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (USkeletalMeshComponent* Weapon = AvatarCharacter->GetWeapon())
		{
			GetCollisionComponents(Weapon, CollisionSocketName);
			BindCallbackToCollision();
		}

		if (APRCharacter* PlayerCharacter = Cast<APRCharacter>(AvatarCharacter))
		{
			UpdateCurrentAttackType(PlayerCharacter);
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBaseWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CurrentAttackType = EAttackType::NormalAttack;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBaseWeaponAbility::ToggleCollision(bool bShouldEnable)
{
	for (const auto& CollisionComp : CollisionComponents)
	{
		CollisionComp->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void UBaseWeaponAbility::GetCollisionComponents(USkeletalMeshComponent* WeaponMesh, const FName& SocketName)
{
	const TArray<USceneComponent*>& Children = WeaponMesh->GetAttachChildren(); 
	
	for (USceneComponent* AttachedComp : Children)
	{
		if (UShapeComponent* ShapeComp = Cast<UShapeComponent>(AttachedComp))
		{
			if (ShapeComp->GetAttachSocketName() == SocketName)
			{
				CollisionComponents.Add(ShapeComp);
			}
		}
	}
}

void UBaseWeaponAbility::BindCallbackToCollision()
{
	for (const auto& CollisionComp : CollisionComponents)
	{
		if (!CollisionComp->OnComponentBeginOverlap.IsBound())
		{
			CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UBaseWeaponAbility::OnOverlap);
		}
	}
}

void UBaseWeaponAbility::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetAvatarActorFromActorInfo() || bHit) return;
	
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		bHit = true;
		CauseDamage(OtherActor);
	}
}

void UBaseWeaponAbility::UpdateCurrentAttackType(APRCharacter* PlayerCharacter)
{
	UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();

	if (PlayerCharacter->bIsCrouching)
	{
		CurrentAttackType = EAttackType::CrouchAttack;
		return;
	}

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
