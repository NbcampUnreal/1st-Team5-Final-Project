// PayRockGames PRGameMode.cpp


#include "PRGameMode.h"
#include "PRGameState.h"
#include "PRIGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "PayRock/Enemy/EnemyGenerator.h"
#include "PayRock/Player/PRPlayerController.h"

APRGameMode::APRGameMode()
{
}

void APRGameMode::BeginPlay()
{
	Super::BeginPlay();
	TotalEnemyCount();
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

void APRGameMode::TotalEnemyCount()
{
	TArray<AActor*> FoundMonsters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyGenerator::StaticClass(), FoundMonsters);

	// GameState에 전달
	if (APRGameState* PRGS = GetGameState<APRGameState>())
	{
		PRGS->SetTotalEnemyCount(FoundMonsters.Num());
	}
}

void APRGameMode::EndThisMatch()
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode: Match is ending..."));

	// 로비로 돌려보내기
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			PC->ClientTravel("/Game/GameSystem/TempMainMenu", TRAVEL_Absolute);
		}
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

void APRGameMode::SpawnAndPossessNecroCharacter(APlayerController* RequestingController, FVector Location)
{
	if (!IsValid(RequestingController))
	{
		UE_LOG(LogTemp, Warning, TEXT("APRGameMode::SpawnAndPossessNecroCharacter(): PlayerController is invalid!"));
		return;
	}

	FVector SpawnLocation = Location;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	/*if (MatchStartClass)
	{
		TArray<AActor*> FoundSpots;
		UGameplayStatics::GetAllActorsOfClass(this, MatchStartClass, FoundSpots);

		if (FoundSpots.Num() != 0)
		{
			AActor* RandomSpot = FoundSpots[FMath::RandRange(0, FoundSpots.Num() - 1)];
			SpawnLocation = RandomSpot->GetActorLocation();
			SpawnRotation = RandomSpot->GetActorRotation();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("APRGameMode::SpawnAndPossessNecroCharacter(): FoundSpots is empty. Spawning at (0, 0, 0)"))
		}
	}*/
	SpawnLocation.Z += 300;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = RequestingController;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* NecroCharacter = GetWorld()->SpawnActor<APawn>(
		NecroCharacterClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (!IsValid(NecroCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("APRGameMode::SpawnAndPossessNecroCharacter(): spawned NecroCharacter is not valid"))
		return;
	}
	RequestingController->UnPossess();
	RequestingController->Possess(NecroCharacter);

	if (APRPlayerController* PlayerController = Cast<APRPlayerController>(RequestingController))
	{
		PlayerController->InitNecroVoice();
	}
}
