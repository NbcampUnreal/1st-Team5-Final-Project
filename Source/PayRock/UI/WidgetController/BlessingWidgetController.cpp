// PayRockGames

#include "BlessingWidgetController.h"
#include "PayRock/Character/Blessing/BlessingDataAsset.h"
#include "PayRock/GameSystem/SaveDataSubsystem.h"

void UBlessingWidgetController::BroadcastInitialValues()
{
	USaveDataSubsystem* SaveSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!SaveSystem) { UE_LOG(LogTemp, Warning, TEXT("SaveDataSubSystem nullptr (BlessingWC)")); return; }
	HandleBlessingSelection(SaveSystem->GetEquippedActiveBlessing());
	HandleBlessingSelection(SaveSystem->GetEquippedPassiveBlessing());
}

void UBlessingWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	USaveDataSubsystem* SaveSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!SaveSystem) { UE_LOG(LogTemp, Warning, TEXT("SaveDataSubSystem nullptr (BlessingWC)")); return; }

	InitializeBlessingContainer(SaveSystem->GetBlessingsContainer());
	SaveSystem->OnBlessingSaved.AddUniqueDynamic(this, &UBlessingWidgetController::InitializeBlessingContainer);
}

void UBlessingWidgetController::HandleBlessingSelection(const FBlessingData& Blessing)
{
	if (Blessing.BlessingName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("BlessingName is empty. <NOT AN ERROR> if unequipped"));
	}

	// Must be able to save this change in the subsystem
	USaveDataSubsystem* SaveSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!SaveSystem) { UE_LOG(LogTemp, Warning, TEXT("SaveDataSubSystem nullptr")); return; }

	switch (Blessing.BlessingType)
	{
	case EBlessingType::Active:
		OnActiveBlessingChange.Broadcast(Blessing);
		SaveSystem->SetEquippedActiveBlessing(Blessing);
		break;

	case EBlessingType::Passive:
		OnPassiveBlessingChange.Broadcast(Blessing);
		SaveSystem->SetEquippedPassiveBlessing(Blessing);
		break;
	}
}

void UBlessingWidgetController::InitializeBlessingContainer(const TArray<FBlessingData>& Blessings)
{
	BlessingsContainer.Empty();
	for (const FBlessingData& Blessing : Blessings)
	{
		BlessingsContainer.Add(Blessing);
	}
}
