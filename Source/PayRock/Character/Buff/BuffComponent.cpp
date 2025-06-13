// PayRockGames


#include "BuffComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/UI/Manager/UIManager.h"


// Sets default values for this component's properties
UBuffComponent::UBuffComponent()
	: OwningPRCharacter(nullptr)
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
	if (!IsValid(OwningPRCharacter)) return;
	
	if (TagCount > 0)
	{
		Client_ApplyDebuffBlind();
	}
	else
	{
		Client_RemoveDebuffBlind();
	}
	/* TODO: broadcast application/removal of debuff */
}

void UBuffComponent::Client_ApplyDebuffBlind_Implementation()
{
	if (!OwningPRCharacter->IsLocallyControlled()) return;

	if (UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>())
	{
		UIManager->ShowWidget(EWidgetCategory::Blind);
	}
}

void UBuffComponent::Client_RemoveDebuffBlind_Implementation()
{
	if (!OwningPRCharacter->IsLocallyControlled()) return;

	if (UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>())
	{
		UIManager->RemoveWidget(EWidgetCategory::Blind);
	}
}

