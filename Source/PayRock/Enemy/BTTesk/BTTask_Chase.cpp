// PayRockGames


#include "BTTask_Chase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = "Chase Target";
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AICon || !Pawn || !BB) return EBTNodeResult::Failed;

	if (BB->GetValueAsBool("bIsAttacking"))
	{
		return EBTNodeResult::Failed;
	}
	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!Target) return EBTNodeResult::Failed;
	
	ACharacter* Character = Cast<ACharacter>(AICon->GetPawn());
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
        
	AICon->MoveToActor(Target, 50.f); 
	return EBTNodeResult::Succeeded; 
}
