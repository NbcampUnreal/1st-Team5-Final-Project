// PayRockGames

#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"
#include "CombatInterface.h"

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

	if (HasAuthority() && IsValid(GetAbilitySystemComponent()))
	{
		GetAbilitySystemComponent()->ClearAllAbilities();
		
		// Remove ALL active gameplay effects
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer());
		GetAbilitySystemComponent()->RemoveActiveEffects(Query);
	}
	
	MulticastRagdoll(HitDirection);

	SpawnLootContainer();

	PrimaryActorTick.bCanEverTick = false;
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

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!CharacterMesh->IsRegistered())
	{
		UE_LOG(LogTemp, Warning, TEXT("Component not registered"));
		return;
	}
	CharacterMesh->SetCollisionProfileName(FName("Ragdoll"));
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CharacterMesh->SetSimulatePhysics(true);
	CharacterMesh->WakeAllRigidBodies();
	CharacterMesh->bReplicatePhysicsToAutonomousProxy = true;

	FVector Impulse = HitDirection * 10000.f;
	if (HitDirection.Equals(FVector::ZeroVector))
	{
		Impulse = -GetActorForwardVector() * 10000.f;
	}
	GetMesh()->AddImpulseAtLocation(Impulse, GetActorLocation());
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (IsValid(this) && IsValid(GetMesh()))
		{
			GetMesh()->SetSimulatePhysics(false);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
		}
	}, 1.f, false);

	PrimaryActorTick.bCanEverTick = false;
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

bool ABaseCharacter::IsPlayerCharacter() const
{
	return CharacterType == ECharacterType::PlayerCharacter;
}

bool ABaseCharacter::IsBoss() const
{
	return CharacterType == ECharacterType::Boss;
}

bool ABaseCharacter::IsMonster() const
{
	switch (CharacterType)
	{
	case ECharacterType::DefaultMonster:
	case ECharacterType::JangSanTiger:
	case ECharacterType::Kappa:
	case ECharacterType::MarketClown:
	case ECharacterType::OneEyed:
	case ECharacterType::Samurai:
	case ECharacterType::SamuraiStatue:
	case ECharacterType::Skeleton:
	case ECharacterType::Doggebi:
	case ECharacterType::Boss:
		return true;

	default:
		return false;
	}
}