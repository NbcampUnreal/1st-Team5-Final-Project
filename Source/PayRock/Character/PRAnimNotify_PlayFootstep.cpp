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
        UE_LOG(LogTemp, Error, TEXT("[Footstep] foot_l socket NOT FOUND!"));
        return;
    }

    FVector FootLocation = MeshComp->GetSocketLocation(TEXT("foot_l"));

    // ✅ 1. 라인트레이스
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

    DrawDebugLine(
        Owner->GetWorld(),
        FootLocation,
        FootLocation - FVector(0, 0, 200.f),
        FColor::Green,
        false, 2.f, 0,
        1.f
    );

    // ✅ 2. 라인트레이스 성공 여부 확인
    UE_LOG(LogTemp, Log, TEXT("[Footstep] LineTrace %s"), bHit ? TEXT("HIT") : TEXT("FAILED"));

    // ✅ 3. PhysMaterial 유효성 로그
    if (bHit)
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            UE_LOG(LogTemp, Log, TEXT("[Footstep] Hit Actor: %s"), *HitActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[Footstep] No Actor hit!"));
        }

        if (UPrimitiveComponent* HitComp = Hit.GetComponent())
        {
            UE_LOG(LogTemp, Log, TEXT("[Footstep] Hit Comp: %s"), *HitComp->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[Footstep] No Component hit!"));
        }

        if (Hit.PhysMaterial.IsValid())
        {
            UE_LOG(LogTemp, Log, TEXT("[Footstep] PhysMaterial: %s"), *Hit.PhysMaterial->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[Footstep] Hit.PhysMaterial is NULL!"));
        }
    }

    if (Hit.Component.IsValid())
    {
        UMaterialInterface* Mat = Hit.Component->GetMaterial(0);
        if (Mat)
        {
            UE_LOG(LogTemp, Log, TEXT("[Footstep] Hit Component Material: %s"), *Mat->GetName());

            if (UPhysicalMaterial* MatPhys = Mat->GetPhysicalMaterial())
            {
                UE_LOG(LogTemp, Log, TEXT("[Footstep] Material's PhysicalMaterial: %s"), *MatPhys->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[Footstep] Material's PhysicalMaterial is NULL!"));
            }
        }
    }


    // ✅ 4. SurfaceType 확인 (기본 + Fallback 방식)
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
            {
                UE_LOG(LogTemp, Log, TEXT("[Footstep] [Fallback] Using material's PhysMat: %s"), *MatPhys->GetName());
                SurfaceType = MatPhys->SurfaceType;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[Footstep] SurfaceType (Final): %d"), (int32)SurfaceType);

    // ✅ 5. 발소리 사운드 선택
    USoundBase* FootstepSound = PRChar->GetFootstepSoundBySurface(SurfaceType);
    if (!FootstepSound)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Footstep] No FootstepSound found for SurfaceType %d"), (int32)SurfaceType);
        return;
    }

    // ✅ 6. 최종 사운드 재생 위치 및 네트워크 처리
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



