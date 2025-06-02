// PayRockGames


#include "FistAttackAbility.h"
#include "Components/SphereComponent.h"
#include "PayRock/Character/PRCharacter.h"


void UFistAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bHit = false;
	AvatarCharacter = Cast<APRCharacter>(GetAvatarActorFromActorInfo());
	BindCallbackToFistCollision();
}

void UFistAttackAbility::BindCallbackToFistCollision()
{
	if (AvatarCharacter && !AvatarCharacter->RightHandCollisionComp->OnComponentBeginOverlap.IsBound()
		&& !AvatarCharacter->LeftHandCollisionComp->OnComponentBeginOverlap.IsBound())
	{
		AvatarCharacter->RightHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UFistAttackAbility::OnFistOverlap);
		AvatarCharacter->LeftHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UFistAttackAbility::OnFistOverlap);
	}
}

void UFistAttackAbility::ToggleFistCollision(bool bShouldEnable)
{
	if (AvatarCharacter)
	{
		AvatarCharacter->RightHandCollisionComp->SetCollisionEnabled(
			bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		AvatarCharacter->LeftHandCollisionComp->SetCollisionEnabled(
			bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void UFistAttackAbility::OnFistOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetAvatarActorFromActorInfo() || bHit) return;

	FVector TargetForward = OtherActor->GetActorForwardVector().GetSafeNormal();
	FVector ToAttacker = (GetAvatarActorFromActorInfo()->GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();

	float Dot = FVector::DotProduct(TargetForward, ToAttacker);

	bool bIsBackAttack = Dot < -0.5f;
	
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		bHit = true;
		CauseDamage(OtherActor, bIsBackAttack);
	}
}

void UFistAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AvatarCharacter)
	{
		AvatarCharacter->RightHandCollisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &UFistAttackAbility::OnFistOverlap);
		AvatarCharacter->LeftHandCollisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &UFistAttackAbility::OnFistOverlap);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
