// PayRockGames


#include "PRAnimNotify_PlayFootstep.h"
#include "PRCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

void UPRAnimNotify_PlayFootstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    APRCharacter* PRChar = Cast<APRCharacter>(Owner);
    if (!PRChar) return;

    UE_LOG(LogTemp, Warning, TEXT("Footstep Notify Triggered | HasAuthority: %d | IsLocallyControlled: %d | IsInAir: %d | MaxWalkSpeed: %.2f | Velocity: %s"),
        Owner->HasAuthority(),
        PRChar->IsLocallyControlled(),
        PRChar->GetCharacterMovement()->IsFalling(),
        PRChar->GetCharacterMovement()->MaxWalkSpeed,
        *PRChar->GetVelocity().ToString());

    UE_LOG(LogTemp, Warning, TEXT("Footstep Notify Triggered at Time: %f"), Owner->GetWorld()->GetTimeSeconds());

    FVector FootLocation = MeshComp->GetSocketLocation(TEXT("foot_l")); // 또는 foot_r

    DrawDebugLine(Owner->GetWorld(), FootLocation, FootLocation - FVector(0, 0, 50.f), FColor::Red, false, 2.0f);

    // 지면 확인
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);
    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
        Hit, FootLocation, FootLocation - FVector(0, 0, 100.f), ECC_Visibility, Params
    );

    if (!bHit) return;

    EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
    USoundBase* FootstepSound = PRChar->GetFootstepSoundBySurface(SurfaceType);

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