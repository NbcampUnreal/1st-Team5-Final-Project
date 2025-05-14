#include "GA_ChargeAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/SpecialEnemy/JangSanTiger/JasnSanTiger.h"

UGA_ChargeAttack::UGA_ChargeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ChargeAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	Tiger = Cast<AJasnSanTiger>(ActorInfo->AvatarActor.Get());
	if (!Tiger) return;

	if (AAIController* AICon = Cast<AAIController>(Tiger->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", true);
			AICon->StopMovement();
		}
	}
	
	Tiger->BeginTelegraph();

	Tiger->GetWorldTimerManager().SetTimer(
		TelegraphTimerHandle,
		this,
		&UGA_ChargeAttack::OnChargeStart,
		TelegraphDuration,
		false
	);
}

void UGA_ChargeAttack::OnChargeStart()
{
	if (!Tiger) return;

	Tiger->EndTelegraph();
	
	
	const FVector Forward = Tiger->GetActorForwardVector();
	Tiger->LaunchCharacter(Forward * ChargeSpeed, true, true);

	if (Tiger->ChargeCollision)
	{
		Tiger->ChargeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Tiger->ChargeCollision->OnComponentBeginOverlap.AddDynamic(this, &UGA_ChargeAttack::OnChargeHit);
	}

	Tiger->GetWorldTimerManager().SetTimer(
		EndChargeTimerHandle,
		this,
		&UGA_ChargeAttack::EndCharge,
		ChargeDuration,
		false
	);
}

void UGA_ChargeAttack::OnChargeHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 BodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == Tiger || HitActors.Contains(OtherActor)) return;

	HitActors.Add(OtherActor);
	CauseDamage(OtherActor); 
}

void UGA_ChargeAttack::EndCharge()
{
	if (!Tiger) return;

	Tiger->GetCharacterMovement()->StopMovementImmediately();

	if (Tiger->ChargeCollision)
	{
		Tiger->ChargeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Tiger->ChargeCollision->OnComponentBeginOverlap.RemoveAll(this);
	}

	// bIsAttacking 초기화
	if (AAIController* AICon = Cast<AAIController>(Tiger->GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsAttacking", false);
			BB->SetValueAsBool("bInAttackRange", false);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}