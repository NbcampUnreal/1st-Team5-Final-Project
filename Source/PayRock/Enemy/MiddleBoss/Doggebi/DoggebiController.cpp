// PayRockGames


#include "DoggebiController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Enemy/EnemyCharacter.h"

ADoggebiController::ADoggebiController()
{
}

void ADoggebiController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !Actor->IsA(APRCharacter::StaticClass())) return;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
	if (!Enemy) return;
	if (Enemy)
	{
		Enemy->SetBattleState(true);
	}

	const FVector AIPos = Enemy->GetActorLocation();

	const FAISenseID HearingID = UAISense::GetSenseID<UAISense_Hearing>();
	const FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
	const FAISenseID DamageID = UAISense::GetSenseID<UAISense_Damage>();

	if (Stimulus.Type == HearingID)
	{
		UE_LOG(LogTemp, Log, TEXT("AI heard sound: %s | Strength: %.2f"), *Actor->GetName(), Stimulus.Strength);

		if (Stimulus.Strength >= LoudnessThreshold)
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), true);
			GetWorld()->GetTimerManager().ClearTimer(ForgetPlayerTimerHandle);
		}
		else if (Stimulus.Strength >= MinLoudnessToReact)
		{
			FRotator LookRot = (Stimulus.StimulusLocation - AIPos).Rotation();
			Enemy->SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
			UE_LOG(LogTemp, Log, TEXT("AI looks toward weak sound at %s"), *Stimulus.StimulusLocation.ToString());
		}
	}
	else if (Stimulus.Type == SightID || Stimulus.Type == DamageID)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);

			FVector TargetPosition =  Actor->GetActorLocation();
			BlackboardComponent->SetValueAsVector(TEXT("TargetPosition"), TargetPosition);
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), true);
			GetWorld()->GetTimerManager().ClearTimer(ForgetPlayerTimerHandle);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(ForgetPlayerTimerHandle, this, &ADoggebiController::ClearDetectedPlayer, 4.0f, false);
		}
	}

}
