// PayRockGames

#include "BTS_CheckPlayerDeath_Stealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Character/PRCharacter.h"

UBTS_CheckPlayerDeath_Stealth::UBTS_CheckPlayerDeath_Stealth()
{
	NodeName = TEXT("Check Player Death or Stealth");
	bNotifyTick = true;
}

void UBTS_CheckPlayerDeath_Stealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	UObject* TargetObj = BB->GetValueAsObject(GetSelectedBlackboardKey());
	if (!TargetObj) return;

	APRCharacter* Player = Cast<APRCharacter>(TargetObj);
	if (!Player) return;
	
	const bool bIsDead = Player->GetbIsDead();
	const bool bIsExtracted = Player->GetbIsExtracted();
	const bool bIsInvisible = Player->GetbIsInvisible();
	
	if (bIsDead || bIsExtracted || bIsInvisible)
	{
		BB->ClearValue(GetSelectedBlackboardKey());
		BB->SetValueAsBool(TEXT("bPlayerDetect"), false);
	}
}
