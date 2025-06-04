#include "BTS_CheckPlayerDeath_Stealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "AIController.h"

UBTS_CheckPlayerDeath_Stealth::UBTS_CheckPlayerDeath_Stealth()
{
	NodeName = TEXT("Check Player Death or Stealth");
}

 void UBTS_CheckPlayerDeath_Stealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/*
	 *UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!Target) return;

	APRCharacter* Player = Cast<APRCharacter>(Target);
	if (!Player || Player->IsPendingKill()) return;

	
	const bool bIsDead = Player->IsDead();
	const bool bIsStealthed = Player->IsInStealth(); 

	if (bIsDead || bIsStealthed)
	{
		BB->ClearValue(GetSelectedBlackboardKey());
	}
	*/
} 
