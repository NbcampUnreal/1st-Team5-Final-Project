// PayRockGames

#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "PayRock/PRGameplayTags.h"

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

FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	const FPRGameplayTags& GameplayTags = FPRGameplayTags::Get();
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	return FVector();
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::AddCharacterAbilities()
{
}

void ABaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float& EffectLevel) const
{
	checkf(IsValid(AbilitySystemComponent), TEXT("ABaseCharacter::ApplyEffectToSelf() - ASC invalid"));
	checkf(IsValid(EffectClass), TEXT("ABaseCharacter::ApplyEffectToSelf() - GE class invalid"))
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass, EffectLevel, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(InitPrimaryAttributeEffect, 1.f);
	ApplyEffectToSelf(InitSecondaryAttributeEffect, 1.f);
	ApplyEffectToSelf(InitVitalAttributeEffect, 1.f);
}