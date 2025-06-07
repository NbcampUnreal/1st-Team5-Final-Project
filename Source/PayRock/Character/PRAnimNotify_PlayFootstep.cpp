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

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
        Hit,
        FootLocation,
        FootLocation - FVector(0, 0, 100.f),
        ECC_WorldStatic,
        Params
    );

    FVector FinalLocation = FootLocation;
    FinalLocation.Z = FMath::Max(FinalLocation.Z - 10.f, 0.f);

    EPhysicalSurface SurfaceType = bHit ? UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()) : SurfaceType_Default;
    USoundBase* FootstepSound = PRChar->GetFootstepSoundBySurface(SurfaceType);

    if (!FootstepSound) return;

    if (Owner->HasAuthority())
    {
        PRChar->MulticastPlayFootstep(FinalLocation, FootstepSound);
    }
    else if (PRChar->IsLocallyControlled())
    {
        PRChar->ServerRequestFootstep(FinalLocation, FootstepSound);
    }
}

