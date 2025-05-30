// PayRockGames

#include "EnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"


AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

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


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	if (AttributeSet)
	{
		AbilitySystemComponent->AddAttributeSetSubobject(Cast<UPRAttributeSet>(AttributeSet));
	}
	
	BindToTagChange();
	
	Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoInitialized();
}

void AEnemyCharacter::AddCharacterAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		HasAuthority(), AbilitySystemComponent != nullptr;
		return;
	}

	for (TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbilities)
	{
		if (StartupAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}
	}
}

void AEnemyCharacter::Die(FVector HitDirection)
{
	Super::Die(HitDirection);

	bIsDead = true;
	

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->CancelAllAbilities();
	}
	
	/*if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.2f);
	}*/
	
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* Brain = AICon->GetBrainComponent())
		{
			Brain->StopLogic(TEXT("Die"));
		}

		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsDead"), true);
			BB->ClearValue("TargetActor");
			BB->SetValueAsBool("bPlayerDetect", false);
			BB->SetValueAsBool("bIsAttacking", false);
		}
		AICon->UnPossess();
	}
	
	/*GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();*/
	
	if (ContainerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(
			ContainerClass,
			GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams
		);
	}
	
	SetLifeSpan(5.0f);
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

bool AEnemyCharacter::IsDead() const
{
	return bIsDead;
}