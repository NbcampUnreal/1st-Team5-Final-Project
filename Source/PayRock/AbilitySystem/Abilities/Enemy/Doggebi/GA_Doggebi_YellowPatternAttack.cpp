// PayRockGames


#include "GA_Doggebi_YellowPatternAttack.h"

#include "AIController.h"
#include "GA_Doggebi_YellowPatternAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

UGA_Doggebi_YellowPatternAttack::UGA_Doggebi_YellowPatternAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Doggebi_YellowPatternAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	ACharacter* Avatar = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	
	Doggebi = Cast<ADoggebiCharacter>(ActorInfo->AvatarActor.Get());
	if (!Doggebi) return;

	if (AAIController* AICon = Cast<AAIController>(Doggebi->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", true);
			AICon->StopMovement();
		}
	}
	BindCallbackToWeaponCollision();
}

void UGA_Doggebi_YellowPatternAttack::BindCallbackToWeaponCollision()
{
	if (Doggebi && !Doggebi->WeaponCollision->OnComponentBeginOverlap.IsBound())
	{
		Doggebi->WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &UGA_Doggebi_YellowPatternAttack::OnWeaponOverlap);
	}
}

void UGA_Doggebi_YellowPatternAttack::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetAvatarActorFromActorInfo()) return;

	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		if (DamagedActors.Contains(OtherActor)) return;

		//CauseDamage(OtherActor);

		DamagedActors.Add(OtherActor);
	}
}

void UGA_Doggebi_YellowPatternAttack::ToggleColiision(bool isActive)
{
	if (Doggebi)
	{
		Doggebi->WeaponCollision->SetCollisionEnabled(
			isActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void UGA_Doggebi_YellowPatternAttack::EndAttackBlackboardState()
{
	if (!Doggebi) return;
	

	if (Doggebi)
	{
		
		if (AAIController* AICon = Cast<AAIController>(Doggebi->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				BB->SetValueAsBool("bIsAttacking", false);
				BB->SetValueAsEnum("bIsNeedMaskChange", true);
			}
		}
		Doggebi->WeaponCollision->OnComponentBeginOverlap.RemoveDynamic(this, &UGA_Doggebi_YellowPatternAttack::UGA_Doggebi_YellowPatternAttack::OnWeaponOverlap);
	}

}


void UGA_Doggebi_YellowPatternAttack::ResetDamaegdActors()
{
	DamagedActors.Empty();
}
