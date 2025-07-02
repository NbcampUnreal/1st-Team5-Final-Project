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

    if (!MeshComp->DoesSocketExist("foot_l"))
    {
        return;
    }

    FVector FootLocation = MeshComp->GetSocketLocation(TEXT("foot_l"));

    // 라인트레이스
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
        Hit,
        FootLocation,
        FootLocation - FVector(0, 0, 200.f),
        ECC_WorldStatic,
        Params
    );

    EPhysicalSurface SurfaceType = SurfaceType_Default;

    if (Hit.PhysMaterial.IsValid())
    {
        SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
    }
    else if (Hit.Component.IsValid()) // fallback
    {
        if (UMaterialInterface* Mat = Hit.Component->GetMaterial(0))
        {
            if (UPhysicalMaterial* MatPhys = Mat->GetPhysicalMaterial())
            {;
                SurfaceType = MatPhys->SurfaceType;
            }
        }
    }

    // 발소리 사운드 선택
    USoundBase* FootstepSound = PRChar->GetFootstepSoundBySurface(SurfaceType);

    // 최종 사운드 재생 위치 및 네트워크 처리
    FVector FinalLocation = FootLocation;
    FinalLocation.Z = FMath::Max(FinalLocation.Z - 10.f, 0.f);

    if (Owner->HasAuthority())
    {
        PRChar->MulticastPlayFootstep(FinalLocation, FootstepSound);
    }
    else if (PRChar->IsLocallyControlled())
    {
        PRChar->ServerRequestFootstep(FinalLocation, FootstepSound);
    }
}



