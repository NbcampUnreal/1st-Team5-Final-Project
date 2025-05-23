// PayRockGames


#include "MarketClownMonsterController.h"

#include "MarketClownMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


void AMarketClownMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AMarketClownMonster* Clown = Cast<AMarketClownMonster>(InPawn);
	if (!Clown || !DefaultBehaviorTree) return;

	Clown->InitAbilityActorInfo();
	Clown->InitializeDefaultAttributes();
	Clown->AddCharacterAbilities();
	
	if (Clown->bIsClone)
	{
		Clown->ApplySplitLevelAttributes(Clown->SplitLevel);
	}

	UBlackboardComponent* BBComponent = nullptr;
	if (UseBlackboard(DefaultBehaviorTree->BlackboardAsset, BBComponent))
	{
		BlackboardComponent = BBComponent;
		RunBehaviorTree(DefaultBehaviorTree);

		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsVector(FName("StartPosition"), Clown->GetActorLocation());
		}
	}
	
	int32 StrategyValue = static_cast<int32>(EClownStrategyType::Aggressive);
	switch (Clown->CurrentMask)
	{
	case ETalMaskType::Yangban:    StrategyValue = static_cast<int32>(EClownStrategyType::Defensive); break;
	case ETalMaskType::Imae:       StrategyValue = static_cast<int32>(EClownStrategyType::Aggressive); break;
	case ETalMaskType::Baekjeong:  StrategyValue = static_cast<int32>(EClownStrategyType::Berserk); break;
	case ETalMaskType::Bune:       StrategyValue = static_cast<int32>(EClownStrategyType::Ranged); break;
	}
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsInt(StrategyKeyName, StrategyValue);
	}
}


