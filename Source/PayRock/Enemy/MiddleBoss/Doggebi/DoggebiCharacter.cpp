// PayRockGames


#include "DoggebiCharacter.h"

#include "DoggebiController.h"
#include "Components/CapsuleComponent.h"

ADoggebiCharacter::ADoggebiCharacter()
{
	AIControllerClass = ADoggebiController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	Mask = CreateDefaultSubobject<USkeletalMeshComponent>("Mask");
	Mask->SetupAttachment(GetMesh(), MaskSocketName);
	Mask->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BatCollision"));
	WeaponCollision->SetupAttachment(Weapon);
}

void ADoggebiCharacter::BeginPlay()
{
	Super::BeginPlay();
	//AttachWeaponChangeSocket(BeforeCombatWeaponSocket);

	AttachWeaponChangeSocket(WeaponSocketName);
	
	if (Mask)
	{
		UMaterialInterface* BaseMat = Mask->GetMaterial(0);
		DynamicMaskMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
		if (DynamicMaskMaterial)
		{
			Mask->SetMaterial(0, DynamicMaskMaterial);
		}
	}
	
	GetWorldTimerManager().SetTimer(MaskChangeTimer, this, &ADoggebiCharacter::CycleMask, 3.0f, true);

	
}

void ADoggebiCharacter::CycleMask()
{
	EMaskType NextMask = EMaskType::Red;

	switch (CurrentMaskIndex)
	{
	case 0:
		NextMask = EMaskType::Red;
		break;
	case 1:
		NextMask = EMaskType::Yellow;
		break;
	case 2:
		NextMask = EMaskType::Blue;
		break;
	}

	ChangeMask(NextMask);

	CurrentMaskIndex = (CurrentMaskIndex + 1) % 3;
}

void ADoggebiCharacter::AttachWeaponChangeSocket(FName NewSocketName)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, NewSocketName);
}

void ADoggebiCharacter::ChangeMask(EMaskType NewMask)
{
	CurrentMask = NewMask;

	if (!DynamicMaskMaterial) return;

	float MaskStateValue = 0.f;

	switch (NewMask)
	{
	case EMaskType::Red:
		MaskStateValue = 0.f;
		break;
	case EMaskType::Yellow:
		MaskStateValue = 1.f;
		break;
	case EMaskType::Blue:
		MaskStateValue = 2.f;
		break;
	}

	DynamicMaskMaterial->SetScalarParameterValue(TEXT("MaskState"), MaskStateValue);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("가면 교체_ChangeMask "));
}


void ADoggebiCharacter::TryExecutePattern()
{
}

void ADoggebiCharacter::OnPatternEnd()
{
	
}


