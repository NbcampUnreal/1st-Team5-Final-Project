

#include "AnimNotify_LaunchToTarget.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

void UAnimNotify_LaunchToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner());
	if (!Enemy) return;

	AAIController* AICon = Cast<AAIController>(Enemy->GetController());
	if (!AICon) return;

	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (!BB) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
	if (!Target) return;
	
	FVector LaunchDir = (Target->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
	FRotator LookAtRot = (Target->GetActorLocation() - Enemy->GetActorLocation()).Rotation();
	FVector LaunchVelocity = LaunchDir * HorizontalSpeed;
	LaunchVelocity.Z = VerticalBoost;
	
	Enemy->SetActorRotation(LookAtRot);
	Enemy->LaunchCharacter(LaunchVelocity, true, true);
}
