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

void ABaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float& EffectLevel) const
{
	checkf(IsValid(AbilitySystemComponent), TEXT("ABaseCharacter::ApplyEffectToSelf() - ASC invalid"));
	checkf(IsValid(EffectClass), TEXT("ABaseCharacter::ApplyEffectToSelf() - GE class invalid"))
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass, EffectLevel, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(InitPrimaryAttributeEffect, 1.f);
	ApplyEffectToSelf(InitSecondaryAttributeEffect, 1.f);
}