// PayRockGames


#include "BuffComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/GameSystem/SaveDataSubsystem.h"
#include "PayRock/UI/Manager/UIManager.h"

UBuffComponent::UBuffComponent()
	: OwningPRCharacter(nullptr), KnockbackForce(800.f), KnockbackVertical(450.f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPRCharacter = Cast<APRCharacter>(GetOwner());
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
		UPRAbilitySystemComponent* ASC =
			Cast<UPRAbilitySystemComponent>(OwningPRCharacter->GetAbilitySystemComponent());
		
		FVector HitDirection = ASC->StoredHitDirection;
		FVector LaunchVelocity = HitDirection * KnockbackForce;
		LaunchVelocity.Z = KnockbackVertical;

		OwningPRCharacter->LaunchCharacter(LaunchVelocity, true, true);
	}

	Client_BroadcastTagChange(Tag, TagCount > 0);
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
		CancelActiveAbilities();
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
		CancelActiveAbilities();
		DisableMovement();
		
		USaveDataSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
		if (!Subsystem) return;
		const UBuffDataAsset* BuffData = Subsystem->GetBuffDataAsset();
		if (!BuffData) return;
		UAnimMontage* Montage = BuffData->GetReactMontageByTag(Tag);
		if (!Montage) return;
		float Duration = OwningPRCharacter->PlayAnimMontage(Montage);
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


