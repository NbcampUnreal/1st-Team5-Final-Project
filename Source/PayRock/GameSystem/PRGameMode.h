// PayRockGames PRGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PRGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API APRGameMode : public AGameMode
{
	GENERATED_BODY()
public: // For Class Member variable

protected: // For Class Member variable

	UPROPERTY()
	TArray<AActor*> UsedMatchStartSpots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	TSubclassOf<AActor> MatchStartClass;

	
public: // For Class Member Function
	
	virtual AActor* ChooseMatchStartSpot();

	void EndThisMatch();
protected: // For Class Member Function

	APRGameMode();
	
	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;

};
