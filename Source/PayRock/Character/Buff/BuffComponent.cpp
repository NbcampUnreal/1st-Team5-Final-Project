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

void UBuffComponent::OnDebuffBlindChanged(const FGameplayTag Tag, int32 TagCount)
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
}

void UBuffComponent::OnDebuffKnockbackChanged(const FGameplayTag Tag, int32 TagCount)
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
	
	if (OwningPRCharacter->HasAuthority())
	{
		Multicast_ReactToTagChange(Tag, TagCount > 0);
	}
}

void UBuffComponent::Multicast_ReactToTagChange_Implementation(const FGameplayTag& Tag, int32 TagCount)
{
	if (!IsValid(OwningPRCharacter)) return;

	/*
	 *	Broadcast for UI
	 */
	if (OwningPRCharacter->IsLocallyControlled())
	{
		//...
	}
	
	/*
	 *	Play React Montage from data asset or Stop if not newly applied
	 */
	if (TagCount > 0)
	{
		USaveDataSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
		if (!Subsystem) return;
		const UBuffDataAsset* BuffData = Subsystem->GetBuffDataAsset();
		if (!BuffData) return;
		UAnimMontage* Montage = BuffData->GetReactMontageByTag(Tag);
		if (!Montage) return;
		OwningPRCharacter->PlayAnimMontage(Montage);
		DisableMovement();
	}
	else
	{
		OwningPRCharacter->StopAnimMontage();
		EnableMovement();
	}
}

void UBuffComponent::DisableMovement()
{
	/*
	 *	Disable movement for the duration of montage / set timer to re-enable movement
	 */
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


