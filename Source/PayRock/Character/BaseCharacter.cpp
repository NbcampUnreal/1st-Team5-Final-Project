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
		return nullptr;
	}

	return HitReactMontages[FMath::RandRange(0, HitReactMontages.Num() - 1)];
}

void ABaseCharacter::Die(FVector HitDirection)
{
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (HasAuthority())
	{
		GetAbilitySystemComponent()->ClearAllAbilities();
		
		// Remove ALL active gameplay effects
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer());
		GetAbilitySystemComponent()->RemoveActiveEffects(Query);
	}
	
	MulticastRagdoll(HitDirection);
}

void ABaseCharacter::MulticastRagdoll_Implementation(const FVector& HitDirection)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.2f);
	}
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!CharacterMesh->IsRegistered())
	{
		UE_LOG(LogTemp, Warning, TEXT("Component not registered yet"));
		return;
	}
	CharacterMesh->SetSimulatePhysics(true);
	CharacterMesh->SetCollisionProfileName(FName("Ragdoll"));
	CharacterMesh->WakeAllRigidBodies();

	FVector Impulse = HitDirection * 10000.f;
	if (HitDirection.Equals(FVector::ZeroVector))
	{
		Impulse = -GetActorForwardVector() * 10000.f;
	}
	GetMesh()->AddImpulseAtLocation(Impulse, GetActorLocation());
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		SpawnLootContainer();
	}, 0.5f, false);
}

void ABaseCharacter::ForceDeath()
{
	Die();
}

UAnimMontage* ABaseCharacter::GetDeathMontage()
{
	if (DeathMontages.IsEmpty()) return nullptr;

	return DeathMontages[FMath::RandRange(0, DeathMontages.Num() - 1)];
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

