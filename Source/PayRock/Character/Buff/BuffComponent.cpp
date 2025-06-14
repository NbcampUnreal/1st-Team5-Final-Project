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
	/* TODO: broadcast application/removal of debuff */
}



