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

	APRCharacter* FocusedPlayer = Cast<APRCharacter>(TargetObj);
	if (!FocusedPlayer) return;
	
	const bool bIsDead = FocusedPlayer->GetbIsDead();
	const bool bIsExtracted = FocusedPlayer->GetbIsExtracted();
	const bool bIsInvisible = FocusedPlayer->GetbIsInvisible();

	if (bIsDead || bIsExtracted || bIsInvisible)
	{
		BB->ClearValue(GetSelectedBlackboardKey());
		
		bool bAnyValidTarget = false;

		for (TObjectIterator<APRCharacter> It; It; ++It)
		{
			if (!IsValid(*It)) continue;
			APRCharacter* OtherPlayer = *It;
			
			if (!OtherPlayer->GetbIsDead() &&
				!OtherPlayer->GetbIsExtracted() &&
				!OtherPlayer->GetbIsInvisible())
			{
				bAnyValidTarget = true;
				break;
			}
		}

		BB->SetValueAsBool(TEXT("bPlayerDetect"), bAnyValidTarget);
	}
}
