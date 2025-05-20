// PayRockGames


#include "PRAnimNotify_PlayFootstep.h"
#include "PRCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Actor.h"

void UPRAnimNotify_PlayFootstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    APRCharacter* PRChar = Cast<APRCharacter>(Owner);
    if (!PRChar) return;

    FVector FootLocation = MeshComp->GetSocketLocation(TEXT("foot_l")); // �Ǵ� foot_r

    // ���� Ȯ��
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);
    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
        Hit, FootLocation, FootLocation - FVector(0, 0, 100.f), ECC_Visibility, Params
    );

    if (!bHit) return;

    // ǥ�� Ÿ������ �߼Ҹ� ����
    EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
    USoundBase* FootstepSound = PRChar->GetFootstepSoundBySurface(SurfaceType); // �� �Լ� �װ� �������� ��

    if (!FootstepSound) return;

    if (Owner->HasAuthority())
    {
        PRChar->MulticastPlayFootstep(Hit.Location, FootstepSound);
    }
    else
    {
        PRChar->ServerRequestFootstep(Hit.Location, FootstepSound);
    }
}