// PayRockGames


#include "UIManager.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"

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

void UUIManager::ShowWidget(EWidgetCategory Category)
{
	UUserWidget* Widget = FindWidget(Category);
	if (!Widget)
	{
		Widget = InitializeWidget(Category);
	}
	ensure(Widget);
	Widget->SetVisibility(ESlateVisibility::HitTestInvisible);
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

	if (Category == EWidgetCategory::Blessing)
	{
		if (UBaseUserWidget* BlessingWidget = Cast<UBaseUserWidget>(Widget))
		{
			APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
			APRPlayerState* PS = PC->GetPlayerState<APRPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
			BlessingWidget->SetWidgetController(GetBlessingWidgetController(WCParams));

			BlessingWidgetController->BroadcastInitialValues();
		}
	}

	if (Widget)
	{
		Widget->AddToViewport();
		WidgetMap.Add(Category, Widget);
	}
	
	return Widget;
}

UBaseWidgetController* UUIManager::GetBlessingWidgetController(const FWidgetControllerParams& WCParams)
{
	if (BlessingWidgetController == nullptr)
	{
		UClass* LoadedClass = WidgetClassData->BlessingWidgetControllerClass.LoadSynchronous();
		if (!LoadedClass) return nullptr;
		BlessingWidgetController = NewObject<UBaseWidgetController>(this, LoadedClass);
		BlessingWidgetController->SetWidgetControllerParams(WCParams);
		BlessingWidgetController->BindCallbacksToDependencies();
	}
	return BlessingWidgetController;
}
