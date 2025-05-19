// PayRockGames


#include "GA_EnemyNormalAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UGA_EnemyNormalAttack::UGA_EnemyNormalAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_EnemyNormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bHit = false;
	ACharacter* Avatar = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	
	samurai = Cast<ASamuraiStatueCharacter>(ActorInfo->AvatarActor.Get());
	if (!Avatar) return;

	if (AAIController* AICon = Cast<AAIController>(Avatar->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", true);
			AICon->StopMovement();
		}
	}
	BindCallbackToWeaponCollision();
	
}

void UGA_EnemyNormalAttack::BindCallbackToWeaponCollision()
{
	if (samurai && !samurai->WeaponCollision->OnComponentBeginOverlap.IsBound())
	{
		samurai->WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &UGA_EnemyNormalAttack::OnWeaponOverlap);
	}
}

void UGA_EnemyNormalAttack::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetAvatarActorFromActorInfo() || bHit) return;
	
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		bHit = true;
		CauseDamage(OtherActor);
	}
}

void UGA_EnemyNormalAttack::ToggleColiision(bool isActive)
{
	if (samurai)
	{
		samurai->WeaponCollision->SetCollisionEnabled(
			isActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void UGA_EnemyNormalAttack::EndAttackBlackboardState()
{
	if (!samurai) return;
	

	if (samurai)
	{
		samurai->WeaponCollision->OnComponentBeginOverlap.RemoveDynamic(this, &UGA_EnemyNormalAttack::UGA_EnemyNormalAttack::OnWeaponOverlap);
	}

}
