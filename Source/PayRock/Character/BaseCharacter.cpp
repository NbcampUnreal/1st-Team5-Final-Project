// PayRockGames

#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/PRGameplayTags.h"

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
			for (const auto& Ability : GetAbilitySystemComponent()->GetActivatableAbilities())
			{
				if (Ability.GetDynamicSpecSourceTags().HasTagExact(FPRGameplayTags::Get().Effects_HitReact))
				{
					GetAbilitySystemComponent()->ClearAbility(Ability.Handle);
				}
			}
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
