// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "WidgetClassData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

UCLASS(BlueprintType)
class PAYROCK_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidget(EWidgetCategory Category);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideWidget(EWidgetCategory Category);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveWidget(EWidgetCategory Category);
	UFUNCTION(BlueprintPure, Category = "UI")
	UUserWidget* FindWidget(EWidgetCategory Category);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UBaseWidgetController* GetBlessingWidgetController(const FWidgetControllerParams& WCParams);

private:
	UUserWidget* InitializeWidget(EWidgetCategory Category);
	
	UPROPERTY()
	TMap<EWidgetCategory, UUserWidget*> WidgetMap;
	UPROPERTY()
	UWidgetClassData* WidgetClassData;
	UPROPERTY()
	UBaseWidgetController* BlessingWidgetController;
};
