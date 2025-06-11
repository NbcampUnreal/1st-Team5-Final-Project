

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
	const float RandomYaw = FMath::FRandRange(-25.f, 25.f);
	LaunchDir = LaunchDir.RotateAngleAxis(RandomYaw, FVector::UpVector);

	
	FVector LaunchVelocity = LaunchDir * FMath::FRandRange(HorizontalSpeed * 0.8f, HorizontalSpeed * 1.2f);
	LaunchVelocity.Z = FMath::FRandRange(VerticalBoost * 1.2f, VerticalBoost * 1.6f); 

	
	const FRotator LookAtRot = LaunchDir.Rotation();
	Enemy->SetActorRotation(LookAtRot + FRotator(0.f, FMath::FRandRange(-10.f, 10.f), 0.f));

	
	Enemy->LaunchCharacter(LaunchVelocity, true, true);
}
