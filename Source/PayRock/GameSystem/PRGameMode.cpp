// PayRockGames PRGameMode.cpp


#include "PRGameMode.h"
#include "PRGameState.h"
#include "PRIGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Pawn.h"

APRGameMode::APRGameMode()
{
}

void APRGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void APRGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	AGameStateBase* GS = GetGameState<AGameStateBase>();

	if (GS && GS->GetClass()->ImplementsInterface(UPRIGameState::StaticClass()))
	{
		IPRIGameState::Execute_Notify_PlayerConnection(GS);
	}
}


void APRGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AGameStateBase* GS = GetGameState<AGameStateBase>();

	if (GS && GS->GetClass()->ImplementsInterface(UPRIGameState::StaticClass()))
	{
		IPRIGameState::Execute_Notify_PlayerConnection(GS);
	}
}

AActor* APRGameMode::ChooseMatchStartSpot()
{
	if (!MatchStartClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MatchStartClass is NULL!"));
		return nullptr;
	}


	TArray<AActor*> FoundSpots;
	UGameplayStatics::GetAllActorsOfClass(this, MatchStartClass, FoundSpots);

	// 사용되지 않은 스폰 지점 필터링
	TArray<AActor*> Available;
	for (AActor* Spot : FoundSpots)
	{
		if (Spot && !UsedMatchStartSpots.Contains(Spot))
		{
			Available.Add(Spot);
		}
	}

	if (Available.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Avaliable MatchStartSpot to use."));
		return nullptr;
	}

	AActor* Chosen = Available[FMath::RandRange(0, Available.Num() - 1)];
	UsedMatchStartSpots.Add(Chosen);
	return Chosen;
}

