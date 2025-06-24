// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "WidgetClassData.h"
#include "PayRock/UI/Widget/Skill/SkillData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class USkillDataAsset;
class UTranslateDataAsset;

UCLASS(BlueprintType)
class PAYROCK_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* ShowWidget(EWidgetCategory Category);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideWidget(EWidgetCategory Category);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveWidget(EWidgetCategory Category);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideAllWidgets();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveAllWidgets();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveAllWidgetsInLobby();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveAllWidgetControllers();
	UFUNCTION(BlueprintPure, Category = "UI")
	UUserWidget* FindWidget(EWidgetCategory Category);
	UFUNCTION()
	void ShowLoadingScreen(const FString& MapName);
	UFUNCTION()
	void RemoveLoadingScreen(UWorld* LoadedWorld);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UBaseWidgetController* GetWidgetController(const FWidgetControllerParams& WCParams, EWidgetCategory Category);

	UFUNCTION(BlueprintCallable, Category = "UI|Translation")
	FString TranslateEnglishToKorean(const FString& InString);

	UFUNCTION(BlueprintCallable, Category = "UI|Skill")
	FSkillData GetSkillData(FName ItemID);

private:
	UUserWidget* InitializeWidget(EWidgetCategory Category);
	void InitializeWidgetController(UUserWidget* Widget, EWidgetCategory Category);
	
	UPROPERTY()
	TMap<EWidgetCategory, UUserWidget*> WidgetMap;
	UPROPERTY()
	TMap<EWidgetCategory, UBaseWidgetController*> WidgetControllerMap;
	UPROPERTY()
	UWidgetClassData* WidgetClassData;

	UPROPERTY()
	UTranslateDataAsset* TranslateDataAsset;

	UPROPERTY()
	USkillDataAsset* SkillDataAsset;
};
