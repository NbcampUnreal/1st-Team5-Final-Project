// PayRockGames

#include "BlessingWidgetController.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Character/Blessing/BlessingComponent.h"
#include "PayRock/GameSystem/SaveDataSubsystem.h"

void UBlessingWidgetController::BroadcastInitialValues()
{
	InitializeBlessingComponentRef();
	if (!BlessingComponent) { UE_LOG(LogTemp, Warning, TEXT("BlessingComponent nullptr")); return; }
	
	BroadcastActiveBlessingChange(BlessingComponent->GetEquippedActiveBlessingData());
	BroadcastPassiveBlessingChange(BlessingComponent->GetEquippedPassiveBlessingData());
}

void UBlessingWidgetController::InitializeBlessingComponentRef()
{
	if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
	{
		BlessingComponent = Character->BlessingComponent;
	}
}

void UBlessingWidgetController::LoadBlessingsFromSubsystem()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown) return;
	
	USaveDataSubsystem* SaveSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!SaveSystem) { UE_LOG(LogTemp, Warning, TEXT("SaveDataSubSystem nullptr")); return; }
	
	for (UBlessingDataAsset* BlessingData : SaveSystem->GetBlessingContainer())
	{
		BlessingsContainer.Add(BlessingData);
	}
}

void UBlessingWidgetController::BindCallbacksToDependencies()
{
	if (!BlessingComponent) return;

	BlessingComponent->OnActiveBlessingChange.AddUniqueDynamic(
		this, &UBlessingWidgetController::BroadcastActiveBlessingChange);
	BlessingComponent->OnPassiveBlessingChange.AddUniqueDynamic(
		this, &UBlessingWidgetController::BroadcastPassiveBlessingChange);
}

void UBlessingWidgetController::BroadcastActiveBlessingChange(UBlessingDataAsset* BlessingDataAsset)
{
	OnActiveBlessingChange.Broadcast(BlessingDataAsset);
}

void UBlessingWidgetController::BroadcastPassiveBlessingChange(UBlessingDataAsset* BlessingDataAsset)
{
	OnPassiveBlessingChange.Broadcast(BlessingDataAsset);
}
