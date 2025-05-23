// PayRockGames


#include "GA_Doggebi_RedPatternAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

UGA_Doggebi_RedPatternAttack::UGA_Doggebi_RedPatternAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Doggebi_RedPatternAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Doggebi = Cast<ADoggebiCharacter>(ActorInfo->AvatarActor.Get());
	if (!Doggebi) return;

	BindCallbackToFireCollision();

	GetWorld()->GetTimerManager().SetTimer(DamageResetTimer, this, &UGA_Doggebi_RedPatternAttack::ResetDamaegdActors, 1.0f, true);

}

void UGA_Doggebi_RedPatternAttack::BindCallbackToFireCollision()
{
	if (Doggebi && Doggebi->FireCollision
		&& !Doggebi->FireCollision->OnComponentBeginOverlap.IsAlreadyBound(this, &UGA_Doggebi_RedPatternAttack::OnFireOverlap))
	{
		Doggebi->FireCollision->OnComponentBeginOverlap.AddDynamic(this, &UGA_Doggebi_RedPatternAttack::OnFireOverlap);
	}
}


void UGA_Doggebi_RedPatternAttack::OnFireOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Doggebi || !OtherActor || OtherActor == Doggebi || DamagedActors.Contains(OtherActor)) return;


	//otheractor가 앉아있을때는 데미지 안받게
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		DamagedActors.Add(OtherActor);
		//CauseDamage(OtherActor); 
	}
}

void UGA_Doggebi_RedPatternAttack::ToggleColiision(bool isActive)
{
	if (Doggebi)
	{
		Doggebi->FireCollision->SetCollisionEnabled(
			isActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void UGA_Doggebi_RedPatternAttack::ResetDamaegdActors()
{
	DamagedActors.Empty();
}

void UGA_Doggebi_RedPatternAttack::EndAttackBlackboardState()
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
		Doggebi->WeaponCollision->OnComponentBeginOverlap.RemoveDynamic(this, &UGA_Doggebi_RedPatternAttack::UGA_Doggebi_RedPatternAttack::OnFireOverlap);
	}

}
