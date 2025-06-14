// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PayRock/UI/WidgetController/BaseWidgetController.h"
#include "WidgetClassData.generated.h"

UENUM(BlueprintType)
enum class EWidgetCategory : uint8
{
	Lobby,
	Stash,
	Blessing,
	LobbyOption,
	Loading,
	MatchHUD,
	InGameHUD,
	Stat,
	Blind,
	MAX
};

UCLASS(Blueprintable, BlueprintType)
class PAYROCK_API UWidgetClassData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<EWidgetCategory, TSoftClassPtr<UUserWidget>> WidgetClassMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<EWidgetCategory, TSoftClassPtr<UBaseWidgetController>> WidgetControllerClassMap;
};
