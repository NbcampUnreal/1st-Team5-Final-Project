// PayRockGames


#include "MarketClownMonsterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MarketClownMonster.h"

void AMarketClownMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMarketClownMonster* Clown = Cast<AMarketClownMonster>(InPawn);
	if (!Clown || !Blackboard) return;

	int32 StrategyValue = static_cast<int32>(EClownStrategyType::Aggressive); 

	switch (Clown->CurrentMask)
	{
	case ETalMaskType::Yangban:
		StrategyValue = static_cast<int32>(EClownStrategyType::Defensive);
		break;
	case ETalMaskType::Imae:
		StrategyValue = static_cast<int32>(EClownStrategyType::Aggressive);
		break;
	case ETalMaskType::Baekjeong:
		StrategyValue = static_cast<int32>(EClownStrategyType::Berserk);
		break;
	case ETalMaskType::Bune:
		StrategyValue = static_cast<int32>(EClownStrategyType::Ranged);
		break;
	default:
		break;
	}

	Blackboard->SetValueAsInt(StrategyKeyName, StrategyValue);
}
