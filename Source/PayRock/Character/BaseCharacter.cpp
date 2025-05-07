// PayRockGames

#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::InitPrimaryAttributes() const
{
	checkf(IsValid(AbilitySystemComponent), TEXT("ABaseCharacter::InitPrimaryAttributes() - ASC invalid - check BP"));
	checkf(IsValid(InitPrimaryAttributeEffect), TEXT("ABaseCharacter::InitPrimaryAttributes() - GE invalid - check BP"))
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		InitPrimaryAttributeEffect, 1.f, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

