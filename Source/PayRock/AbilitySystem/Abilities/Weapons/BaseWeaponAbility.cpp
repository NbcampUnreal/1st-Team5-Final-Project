// PayRockGames

#include "BaseWeaponAbility.h"
#include "Components/ShapeComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	AlreadyHitActors.Empty();
	if (ABaseCharacter* AvatarCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (CollisionComponents.IsEmpty())
		{
			if (USkeletalMeshComponent* Weapon = AvatarCharacter->GetWeapon())
			{
				GetCollisionComponents(Weapon, CollisionSocketName);
				BindCallbackToCollision();
			}
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


void UBaseWeaponAbility::ToggleCollision(bool bShouldEnable)
{
	if (bShouldEnable)
	{
		AlreadyHitActors.Empty();
	}
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
	if (OtherActor == GetAvatarActorFromActorInfo() || AlreadyHitActors.Contains(OtherActor)) return;

	AlreadyHitActors.Add(OtherActor);
	FVector TargetForward = OtherActor->GetActorForwardVector().GetSafeNormal();
	FVector ToAttacker = (GetAvatarActorFromActorInfo()->GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();

	float Dot = FVector::DotProduct(TargetForward, ToAttacker);

	bool bIsBackAttack = Dot < -0.5f;
	
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		CauseDamage(OtherActor, SweepResult, bIsBackAttack);
	}
}

void UBaseWeaponAbility::UpdateCurrentAttackType(APRCharacter* PlayerCharacter)
{
	UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement();

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
