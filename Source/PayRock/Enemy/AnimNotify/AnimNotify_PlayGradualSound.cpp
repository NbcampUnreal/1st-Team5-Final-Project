// PayRockGames

#include "AnimNotify_PlayGradualSound.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UAnimNotify_PlayGradualSound::UAnimNotify_PlayGradualSound()
{
	Sound = nullptr;
	MaxHearingDistance = 2000.f;
	BaseVolume = 1.0f;
	LoudnessForAI = 1.0f;
}

void UAnimNotify_PlayGradualSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!Sound || !MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner || !Owner->HasAuthority()) return;
	
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Owner))
	{
		Enemy->PlayGradualSound(Sound, MaxHearingDistance, BaseVolume, LoudnessForAI);
	}
}
