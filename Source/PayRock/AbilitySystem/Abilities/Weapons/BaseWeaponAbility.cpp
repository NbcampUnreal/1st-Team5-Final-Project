// PayRockGames

#include "BaseWeaponAbility.h"
#include "Components/BoxComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] ActivateAbility - Clearing AlreadyHitActors"), 
	GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER"));
	AlreadyHitActors.Empty();
	if (ABaseCharacter* AvatarCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (APRCharacter* PlayerCharacter = Cast<APRCharacter>(AvatarCharacter))
		{
			CollisionComponents.Empty();
			CollisionComponents.Add(PlayerCharacter->WeaponCollision);
			UpdateCurrentAttackType(PlayerCharacter);
		}
		else
		{
			if (USkeletalMeshComponent* Weapon = AvatarCharacter->GetWeapon())
			{
				GetCollisionComponents(Weapon, CollisionSocketName);
			}
		}
		BindCallbackToCollision();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBaseWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] EndAbility - Clearing AlreadyHitActors"), 
	GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER"));
	for (const auto& CollisionComp : CollisionComponents)
	{
		if (CollisionComp->OnComponentBeginOverlap.IsBound())
		{
			CollisionComp->OnComponentBeginOverlap.RemoveAll(this);
		}
	}
	
	CurrentAttackType = EAttackType::NormalAttack;
	AlreadyHitActors.Empty();
	CollisionComponents.Empty();

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
	if (!bShouldEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ToggleCollision(false) - Clearing AlreadyHitActors"), 
			GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER"));
		AlreadyHitActors.Empty();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ToggleCollision(true) - NOT clearing AlreadyHitActors"), 
			GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER"));
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
	if (!IsValid(OtherActor) || OtherActor == GetAvatarActorFromActorInfo()) return;

	/*UE_LOG(LogTemp, Warning, TEXT("[%s] OnOverlap with %s - AlreadyHit: %s - Authority: %s"), 
	GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER"),
	*OtherActor->GetName(),
	AlreadyHitActors.Contains(OtherActor) ? TEXT("YES") : TEXT("NO"),
	GetAvatarActorFromActorInfo()->HasAuthority() ? TEXT("YES") : TEXT("NO"));*/

	
	UE_LOG(LogTemp, Warning, TEXT("[%s] OnOverlap Details:"), 
		GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER"));
	UE_LOG(LogTemp, Warning, TEXT("  - Target: %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("  - Collision Component: %s"), *OverlappedComponent->GetName());
	UE_LOG(LogTemp, Warning, TEXT("  - AlreadyHitActors Size: %d"), AlreadyHitActors.Num());
	UE_LOG(LogTemp, Warning, TEXT("  - AlreadyHit: %s"), AlreadyHitActors.Contains(OtherActor) ? TEXT("YES") : TEXT("NO"));
	UE_LOG(LogTemp, Warning, TEXT("  - Authority: %s"), GetAvatarActorFromActorInfo()->HasAuthority() ? TEXT("YES") : TEXT("NO"));

    
	if (AlreadyHitActors.Contains(OtherActor)) 
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("  - ADDING TO HIT LIST"));
	AlreadyHitActors.Add(OtherActor);
	FVector TargetForward = OtherActor->GetActorForwardVector().GetSafeNormal();
	FVector ToAttacker = (GetAvatarActorFromActorInfo()->GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();

	float Dot = FVector::DotProduct(TargetForward, ToAttacker);

	bool bIsBackAttack = Dot < -0.5f;

	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("  - CALLING CauseDamage"));
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
