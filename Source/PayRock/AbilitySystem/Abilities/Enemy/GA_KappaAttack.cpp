#include "GA_KappaAttack.h"
#include "PayRock/Enemy/SpecialEnemy/Kappa/KappaMonster.h"
#include "Components/SphereComponent.h"

void UGA_KappaAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bHit = false;
	AvatarCharacter = Cast<AKappaMonster>(GetAvatarActorFromActorInfo());
	BindCallbackToFistCollision();
}

void UGA_KappaAttack::BindCallbackToFistCollision()
{
	if (!AvatarCharacter) return;

	
	if (AvatarCharacter->LeftHandCollisionComp && 
		!AvatarCharacter->LeftHandCollisionComp->OnComponentBeginOverlap.IsBound())
	{
		AvatarCharacter->LeftHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UGA_KappaAttack::OnFistOverlap);
	}

	
	if (AvatarCharacter->RightHandCollisionComp && 
		!AvatarCharacter->RightHandCollisionComp->OnComponentBeginOverlap.IsBound())
	{
		AvatarCharacter->RightHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UGA_KappaAttack::OnFistOverlap);
	}
}

void UGA_KappaAttack::ToggleFistCollision(bool bShouldEnable)
{
	if (!AvatarCharacter) return;

	ECollisionEnabled::Type CollisionState = bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;

	if (AvatarCharacter->LeftHandCollisionComp)
	{
		AvatarCharacter->LeftHandCollisionComp->SetCollisionEnabled(CollisionState);
	}

	if (AvatarCharacter->RightHandCollisionComp)
	{
		AvatarCharacter->RightHandCollisionComp->SetCollisionEnabled(CollisionState);
	}
}

void UGA_KappaAttack::OnFistOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetAvatarActorFromActorInfo() || bHit) return;

	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		bHit = true;
		CauseDamage(OtherActor);
	}
}

void UGA_KappaAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AvatarCharacter)
	{
		if (AvatarCharacter->LeftHandCollisionComp)
		{
			AvatarCharacter->LeftHandCollisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &UGA_KappaAttack::OnFistOverlap);
		}

		if (AvatarCharacter->RightHandCollisionComp)
		{
			AvatarCharacter->RightHandCollisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &UGA_KappaAttack::OnFistOverlap);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
