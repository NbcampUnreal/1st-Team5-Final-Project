// PayRockGames

#include "PRAnimNotify_AttackSound.h"
#include "PayRock/Character/PRCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/PRGameplayTags.h"

//void UPRAnimNotify_AttackSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
//{
//	if (!MeshComp || !SwingSound) return;
//
//	AActor* Owner = MeshComp->GetOwner();
//	APRCharacter* PRChar = Cast<APRCharacter>(Owner);
//	if (!PRChar || !PRChar->IsLocallyControlled()) return;
//
//	PRChar->ServerPlayAttackSound(SwingSound, MeshComp->GetComponentLocation());
//}

void UPRAnimNotify_AttackSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwingSound] MeshComp is NULL"));
		return;
	}

	if (!SwingSound)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwingSound] Sound is NULL"));
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	APRCharacter* PRChar = Cast<APRCharacter>(Owner);

	if (!PRChar)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwingSound] Owner cast failed"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[SwingSound] Notify Triggered for: %s"), *PRChar->GetName());

	if (PRChar->IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SwingSound] Calling ServerPlayAttackSound"));

		// 먼저 로컬에서 직접 재생 (자기 자신이 들을 수 있도록)
		UGameplayStatics::PlaySoundAtLocation(
			PRChar,
			SwingSound,
			MeshComp->GetComponentLocation(),
			FRotator::ZeroRotator,
			1.0f,
			1.0f,
			0.0f,
			Attenuation
		);

		// 그리고 서버로 요청 → 다른 클라들에게 멀티캐스트
		PRChar->ServerPlayAttackSound(SwingSound, MeshComp->GetComponentLocation());
	}
}