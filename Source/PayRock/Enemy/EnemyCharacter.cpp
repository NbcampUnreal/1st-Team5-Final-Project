// PayRockGames

#include "EnemyCharacter.h"
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