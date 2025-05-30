// PayRockGames


#include "UIManager.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "PayRock/UI/WidgetController/StatWidgetController.h"

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FSoftObjectPath DataAssetPath(TEXT("/Game/UI/Data/DA_WidgetClassData"));
	if (UWidgetClassData* LoadedDataAsset = Cast<UWidgetClassData>(DataAssetPath.TryLoad()))
	{
		WidgetClassData = LoadedDataAsset;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load WidgetClassData!"));
	}
}

UUserWidget* UUIManager::ShowWidget(EWidgetCategory Category)
{
	if (!GetWorld() || GetWorld()->bIsTearingDown) return nullptr;
	
	UUserWidget* Widget = FindWidget(Category);
	if (!Widget)
	{
		Widget = InitializeWidget(Category);
	}
	ensure(Widget);
	Widget->SetVisibility(ESlateVisibility::Visible);
	return Widget;
}

void UUIManager::HideWidget(EWidgetCategory Category)
{
	UUserWidget* Widget = FindWidget(Category);
	if (!Widget) return;
	Widget->SetVisibility(ESlateVisibility::Collapsed);
}

void UUIManager::RemoveWidget(EWidgetCategory Category)
{
	UUserWidget* Widget = nullptr;
	WidgetMap.RemoveAndCopyValue(Category, Widget);
	if (!Widget) return;
	Widget->RemoveFromParent();
}

void UUIManager::HideAllWidgets()
{
	for (uint8 i = 0; i < static_cast<uint8>(EWidgetCategory::MAX); i++)
	{
		HideWidget(static_cast<EWidgetCategory>(i));
	}
}

void UUIManager::RemoveAllWidgets()
{
	for (uint8 i = 0; i < static_cast<uint8>(EWidgetCategory::MAX); i++)
	{
		RemoveWidget(static_cast<EWidgetCategory>(i));
	}
}

void UUIManager::RemoveAllWidgetsInLobby()
{
	for (uint8 i = 0; i < static_cast<uint8>(EWidgetCategory::MAX); i++)
	{
		if (i == static_cast<uint8>(EWidgetCategory::Lobby)) continue;
		RemoveWidget(static_cast<EWidgetCategory>(i));
	}
}

UUserWidget* UUIManager::FindWidget(EWidgetCategory Category)
{
	if (UUserWidget** WidgetPtr = WidgetMap.Find(Category))
	{
		return *WidgetPtr;
	}
	return nullptr;
}

UUserWidget* UUIManager::InitializeWidget(EWidgetCategory Category)
{
	if (!WidgetClassData)
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetClassData is nullptr."));
		return nullptr;
	}
	TSoftClassPtr<UUserWidget>* WidgetClassPtr = WidgetClassData->WidgetClassMap.Find(Category);
	if (!WidgetClassPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetClassMap.Find() failed."));
		return nullptr;
	}
	UClass* WidgetClass = WidgetClassPtr->LoadSynchronous();
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetClassPtr->LoadSynchronous() failed."));
		return nullptr;
	}
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClass);

	InitializeWidgetController(Widget, Category);

	if (Widget)
	{
		Widget->AddToViewport();
		WidgetMap.Add(Category, Widget);
	}
	
	return Widget;
}

void UUIManager::InitializeWidgetController(UUserWidget* Widget, EWidgetCategory Category)
{
	if (UBaseUserWidget* BaseUserWidget = Cast<UBaseUserWidget>(Widget))
	{
		APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
		APRPlayerState* PS = PC->GetPlayerState<APRPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
		
		if (UBaseWidgetController* WidgetController = GetWidgetController(WCParams, Category))
		{
			BaseUserWidget->SetWidgetController(WidgetController);
			WidgetController->BroadcastInitialValues();	
		}
	}
}

UBaseWidgetController* UUIManager::GetWidgetController(const FWidgetControllerParams& WCParams, EWidgetCategory Category)
{
	if (UBaseWidgetController** WidgetControllerPtr = WidgetControllerMap.Find(Category))
	{
		// WidgetController in TMap is valid --> return it
		if (IsValid(*WidgetControllerPtr))
		{
			if (Category == EWidgetCategory::Stat)
			{
				if ((*WidgetControllerPtr)->IsAbilitySystemValid())
				{
					return *WidgetControllerPtr;
				}
			}
			else
			{
				return *WidgetControllerPtr;	
			}
		}
		
		
		// WidgetController in TMap is not valid --> remove it from TMap
		WidgetControllerMap.Remove(Category);
	}
	
	TSoftClassPtr<UBaseWidgetController>* WidgetControllerClassPtr = WidgetClassData->WidgetControllerClassMap.Find(Category);
	if (!WidgetControllerClassPtr) return nullptr; // valid because widgetcontroller doesn't apply to certain widgets
	
	UClass* LoadedClass = WidgetControllerClassPtr->LoadSynchronous();
	if (!LoadedClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load WidgetControllerClass"));
		return nullptr;
	}

	UBaseWidgetController* WidgetController = NewObject<UBaseWidgetController>(this, LoadedClass);
	WidgetController->SetWidgetControllerParams(WCParams);
	WidgetController->BindCallbacksToDependencies();
	WidgetControllerMap.Add(Category, WidgetController);

	return WidgetController;
}
