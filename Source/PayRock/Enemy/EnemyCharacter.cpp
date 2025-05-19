// PayRockGames

#include "EnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"


AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UPRAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UPRAttributeSet>("AttributeSet");
}

void AEnemyCharacter::ToggleWeaponCollision(bool bEnable)
{
	if (Weapon)
	{
		Weapon->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

void AEnemyCharacter::Die()
{
	Super::Die();
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* Brain = AICon->GetBrainComponent())
		{
			Brain->StopLogic("Died");
		}

		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->ClearValue("TargetActor");
			BB->SetValueAsBool("bPlayerDetect", false);
			BB->SetValueAsBool("bIsAttacking", false);
		}

		AICon->UnPossess();
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
	InitializeDefaultAttributes();
	BindToTagChange();
	AddCharacterAbilities();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoInitialized();
}

void AEnemyCharacter::AddCharacterAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
		return;

	for (TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
	}
}

UAnimMontage* AEnemyCharacter::GetRandomAttackMontage() const
{
	if (AttackMontages.Num() == 0) return nullptr;

	int32 Index = FMath::RandRange(0, AttackMontages.Num() - 1);
	return AttackMontages[Index];
}

UAnimMontage* AEnemyCharacter::GetRandomDetectMontage() const
{
	if (DetectMontages.Num() == 0) return nullptr;

	int32 Index = FMath::RandRange(0, DetectMontages.Num() - 1);
	return DetectMontages[Index];
}