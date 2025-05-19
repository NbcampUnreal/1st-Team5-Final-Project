// PayRockGames


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
	if (AvatarCharacter && !AvatarCharacter->HeadCollisionComp->OnComponentBeginOverlap.IsBound())
	{
		AvatarCharacter->HeadCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UGA_KappaAttack::OnFistOverlap);
	}
}

void UGA_KappaAttack::ToggleFistCollision(bool bShouldEnable)
{
	if (AvatarCharacter)
	{
		AvatarCharacter->HeadCollisionComp->SetCollisionEnabled(
			bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
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
		AvatarCharacter->HeadCollisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &UGA_KappaAttack::OnFistOverlap);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
