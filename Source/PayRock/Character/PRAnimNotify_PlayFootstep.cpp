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

    FVector FootLocation = MeshComp->GetSocketLocation(TEXT("foot_l")); // 또는 "foot_r"

    if (!MeshComp->DoesSocketExist("foot_l"))
    {
        UE_LOG(LogTemp, Error, TEXT("[Footstep] foot_l socket NOT FOUND!"));
    }

    // 라인트레이스
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
        Hit,
        FootLocation,
        FootLocation - FVector(0, 0, 100.f),
        ECC_WorldStatic, // 채널 변경: 지면과 충돌하도록
        Params
    );

    UE_LOG(LogTemp, Warning, TEXT("[Footstep] FootSocketLocation: %s | Hit: %d"), *FootLocation.ToString(), bHit);

    // 디버그 시각화 (선택)
    DrawDebugLine(Owner->GetWorld(), FootLocation, FootLocation - FVector(0, 0, 100.f), bHit ? FColor::Green : FColor::Red, false, 1.5f, 0, 1.0f);

    // 최종 발소리 재생 위치
    FVector FinalLocation = FootLocation;
    FinalLocation.Z = FMath::Max(FinalLocation.Z - 10.f, 0.f);

    // 표면 타입 추출 (없으면 기본값)
    EPhysicalSurface SurfaceType = bHit ? UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()) : SurfaceType_Default;
    USoundBase* FootstepSound = PRChar->GetFootstepSoundBySurface(SurfaceType);

    if (!FootstepSound) return;

    // 서버 or 클라이언트에서 발소리 재생 요청
    if (Owner->HasAuthority())
    {
        PRChar->MulticastPlayFootstep(FinalLocation, FootstepSound);
    }
    else
    {
        PRChar->ServerRequestFootstep(FinalLocation, FootstepSound);
    }
}

