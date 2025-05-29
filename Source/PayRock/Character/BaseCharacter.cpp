// PayRockGames

#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Stop blocking the camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), WeaponSocketName);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::BindToTagChange()
{
	if (AbilitySystemComponent)
	{
		// Hit React Binding
		AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Effects_HitReact).AddUObject(
			this, &ABaseCharacter::OnHitReactTagChanged);
	}
}

void ABaseCharacter::OnHitReactTagChanged(const FGameplayTag ChangedTag, int32 TagCount)
{
	bHitReact = TagCount > 0;
}

const UAnimMontage* ABaseCharacter::GetHitReactMontage()
{
	if (HitReactMontages.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("HitReactMontages TArray is empty. Please assign montages in BP."))
		return nullptr;
	}

	return HitReactMontages[FMath::RandRange(0, HitReactMontages.Num() - 1)];
}

void ABaseCharacter::Die(/*const FHitResult& HitResult*/)
{
	if (true) //HasAuthority()
	{
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
	
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (HasAuthority())
		{
			SpawnLootContainer();
		}
		
		/*if (HitResult.bBlockingHit)
		{
			FVector Impulse = -HitResult.ImpactNormal * 10000.f;
			Impulse = Impulse.GetClampedToMaxSize(100000.f);
			GetMesh()->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint, HitResult.BoneName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Die - Not a blocking hit. No Impulse applied"));
		}*/
	}
}

UAnimMontage* ABaseCharacter::GetDeathMontage()
{
	if (DeathMontages.IsEmpty()) return nullptr;

	return DeathMontages[FMath::RandRange(0, DeathMontages.Num() - 1)];
}

void ABaseCharacter::ForceDeath()
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FPRGameplayTags::Get().Status_Life_Dead);
	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
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
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::ApplyEffectToSelf() - ASC invalid")); return;
	}
	if (!IsValid(EffectClass))
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseCharacter::ApplyEffectToSelf() - GE class invalid")); return;
	}
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass, EffectLevel, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ABaseCharacter::InitializeDefaultAttributes()
{
	if (const AMarketClownMonster* Monster = Cast<AMarketClownMonster>(this))
	{
		if (Monster->bIsClone) return; 
	}
	ApplyEffectToSelf(InitPrimaryAttributeEffect, 1.f);
	ApplyEffectToSelf(InitSecondaryAttributeEffect, 1.f);
	ApplyEffectToSelf(InitVitalAttributeEffect, 1.f);
	bAreAttributesInitialized = true;
}
/*
void ABaseCharacter::RecalculateSecondaryAttributesDelayed()
{
	if (bRecalculationScheduled) return;
	bRecalculationScheduled = true;

	GetWorldTimerManager().SetTimer(StatRecalculateTimerHandle,
		this, &ABaseCharacter::RecalculateSecondaryAttributes, 0.5f, false);
}

void ABaseCharacter::RecalculateSecondaryAttributes()
{
	bRecalculationScheduled = false;
	if (!bAreAttributesInitialized) return;
	
	UPRAttributeSet* AS = Cast<UPRAttributeSet>(GetAttributeSet());
	const float OldMaxHealth = AS->GetMaxHealth();
	const float OldMaxMana = AS->GetMaxMana();
	
	ApplyEffectToSelf(RecalculateSecondaryEffect, 1.f);

	const float NewMaxHealth = AS->GetMaxHealth();
	const float NewMaxMana = AS->GetMaxMana();

	if (!FMath::IsNearlyEqual(OldMaxHealth, NewMaxHealth))
	{
		const float Health = AS->GetHealth();
		const float NewHealth = AS->HealthRatio * NewMaxHealth;
		if (!FMath::IsNearlyEqual(Health, NewHealth))
		{
			AS->SetHealth(FMath::Clamp(NewHealth, 0.f, NewMaxHealth));
			UE_LOG(LogTemp, Warning, TEXT("SetHealth called in recalculate function"))
		}
	}
	
	if (!FMath::IsNearlyEqual(OldMaxMana, NewMaxMana))
	{
		const float Mana = AS->GetMana();
		const float NewMana = AS->ManaRatio * NewMaxMana;
		if (!FMath::IsNearlyEqual(Mana, NewMana))
		{
			AS->SetMana(FMath::Clamp(NewMana, 0.f, NewMaxMana));
		}
	}
}
*/
