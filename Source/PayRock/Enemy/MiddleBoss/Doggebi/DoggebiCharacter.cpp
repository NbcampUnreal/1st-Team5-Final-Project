// PayRockGames


#include "DoggebiCharacter.h"

#include "DoggebiController.h"

ADoggebiCharacter::ADoggebiCharacter()
{
	AIControllerClass = ADoggebiController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADoggebiCharacter::BeginPlay()
{
	Super::BeginPlay();
	AttachWeaponChangeSocket(BeforeCombatWeaponSocket);
}

void ADoggebiCharacter::AttachWeaponChangeSocket(FName NewSocketName)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, NewSocketName);
}

void ADoggebiCharacter::ChangeMask(EMaskType NewMask)
{
}


void ADoggebiCharacter::TryExecutePattern()
{
}

void ADoggebiCharacter::OnPatternEnd()
{
}
