// PayRockGames

#include "PRAnimNotify_AttackSound.h"
#include "PayRock/Character/PRCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/PRGameplayTags.h"

void UPRAnimNotify_AttackSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	if (!SwingSound)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	APRCharacter* PRChar = Cast<APRCharacter>(Owner);

	if (!PRChar)
	{
		return;
	}


	if (PRChar->IsLocallyControlled())
	{
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

		PRChar->ServerPlayAttackSound(SwingSound, MeshComp->GetComponentLocation());
	}
}