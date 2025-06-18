// PayRockGames


#include "BuffComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/GameSystem/SaveDataSubsystem.h"
#include "PayRock/UI/Manager/UIManager.h"

UBuffComponent::UBuffComponent()
	: OwningPRCharacter(nullptr), KnockbackForce(800.f), KnockbackVertical(450.f), bIsKnockedBack(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPRCharacter = Cast<APRCharacter>(GetOwner());
}

void UBuffComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (GetWorld() && !GetWorld()->bIsTearingDown)
	{
		GetWorld()->GetTimerManager().ClearTimer(KnockbackRecoveryTimer);
	}
}

void UBuffComponent::OnBlindTagChange(const FGameplayTag Tag, int32 TagCount)
{
	if (!IsValid(OwningPRCharacter) || !OwningPRCharacter->IsLocallyControlled()) return;
	
	if (TagCount > 0)
	{
		if (UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>())
		{
			UIManager->ShowWidget(EWidgetCategory::Blind);
		}
	}
	else
	{
		if (UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>())
		{
			UIManager->RemoveWidget(EWidgetCategory::Blind);
		}
	}

	Client_BroadcastTagChange(Tag, TagCount > 0);
}

void UBuffComponent::OnKnockbackTagChange(const FGameplayTag Tag, int32 TagCount)
{
	if (!IsValid(OwningPRCharacter)) return;

	if (TagCount > 0)
	{
		bIsKnockedBack = true;
		GetWorld()->GetTimerManager().ClearTimer(KnockbackRecoveryTimer);

		// TODO: cancel abilities other than regeneration
		//DisableMovement();
		
		UPRAbilitySystemComponent* ASC =
			Cast<UPRAbilitySystemComponent>(OwningPRCharacter->GetAbilitySystemComponent());
		FVector HitDirection = ASC->StoredHitDirection;
		FVector LaunchVelocity = HitDirection * KnockbackForce;
		LaunchVelocity.Z = KnockbackVertical;
		OwningPRCharacter->SetActorRotation(-HitDirection.ToOrientationQuat());
		OwningPRCharacter->LaunchCharacter(LaunchVelocity, true, true);

		//PlayMontageByTag(Tag); // Looping montage
	}

	Client_BroadcastTagChange(Tag, TagCount > 0);
}

void UBuffComponent::StartKnockbackRecovery()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "StartKnockbackRecovery");
	//if (!GetWorld() || GetWorld()->bIsTearingDown) return;
	
	//float Duration = PlayMontageByTag(FPRGameplayTags::Get().Status_Debuff_Knockback_Recovery);
	//GetWorld()->GetTimerManager().SetTimer(
	//	KnockbackRecoveryTimer, this, &UBuffComponent::KnockbackRecovery, Duration);
}

void UBuffComponent::KnockbackRecovery()
{
	bIsKnockedBack = false;

	if (UAbilitySystemComponent* ASC = OwningPRCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer Tags;
		Tags.AddTagFast(FPRGameplayTags::Get().Status_Debuff_Knockback);
		ASC->RemoveActiveEffectsWithGrantedTags(Tags);
	}
	
	EnableMovement();
}

void UBuffComponent::OnFrozenTagChange(const FGameplayTag Tag, int32 TagCount)
{
	if (!IsValid(OwningPRCharacter)) return;

	USkeletalMeshComponent* Mesh = OwningPRCharacter->GetMesh();
	if (!IsValid(Mesh)) return;
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;
	
	if (TagCount > 0)
	{
		AnimInstance->Montage_Pause();
		Mesh->bPauseAnims = true;
		// CancelActiveAbilities();
		DisableMovement();
	}
	else
	{
		AnimInstance->Montage_Resume(nullptr);
		Mesh->bPauseAnims = false;
		EnableMovement();
	}

	Client_BroadcastTagChange(Tag, TagCount > 0);
}

void UBuffComponent::OnShockedTagChange(const FGameplayTag Tag, int32 TagCount)
{
	if (TagCount > 0)
	{
		// CancelActiveAbilities();
		DisableMovement();
		PlayMontageByTag(Tag);
	}
	else
	{
		OwningPRCharacter->StopAnimMontage();
		EnableMovement();
	}

	Client_BroadcastTagChange(Tag, TagCount > 0);
}

void UBuffComponent::Client_BroadcastTagChange_Implementation(const FGameplayTag& Tag, int32 TagCount)
{
	if (!IsValid(OwningPRCharacter)) return;
	
	if (OwningPRCharacter->IsLocallyControlled())
	{
		// Broadcast for UI
	}
}

float UBuffComponent::PlayMontageByTag(const FGameplayTag Tag)
{
	USaveDataSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!Subsystem) return 0.f;
	const UBuffDataAsset* BuffData = Subsystem->GetBuffDataAsset();
	if (!BuffData) return 0.f;
	UAnimMontage* Montage = BuffData->GetReactMontageByTag(Tag);
	if (!Montage) return 0.f;
	return OwningPRCharacter->PlayAnimMontage(Montage);
}

void UBuffComponent::DisableMovement()
{
	UCharacterMovementComponent* Movement = OwningPRCharacter->GetCharacterMovement();
	if (!Movement) return;
	Movement->StopMovementImmediately();
	Movement->DisableMovement();
}

void UBuffComponent::EnableMovement()
{
	UCharacterMovementComponent* Movement = OwningPRCharacter->GetCharacterMovement();
	if (!Movement) return;
	Movement->SetMovementMode(MOVE_Walking); 
}

void UBuffComponent::CancelActiveAbilities()
{
	if (!IsValid(OwningPRCharacter)) return;
	UAbilitySystemComponent* AbilitySystemComponent = OwningPRCharacter->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent)) return;
	
	AbilitySystemComponent->CancelAllAbilities();
}


