// PayRockGames


#include "MukCheonWangController.h"

#include "MukCheonWangCharacter.h"


AMukCheonWangController::AMukCheonWangController()
{
	bAttachToPawn = true;
}

void AMukCheonWangController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(InPawn);
	if (Boss)
	{
		Boss->InitAbilityActorInfo();
		Boss->InitializeDefaultAttributes();
		Boss->AddCharacterAbilities();
	}
	
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}