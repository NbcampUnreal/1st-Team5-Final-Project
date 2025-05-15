// PayRockGames


#include "PayRock/Character/PRAnimInstance.h"
#include "PayRock/Character/PRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPRAnimInstance::NativeInitializeAnimation()
{
    OwnerCharacter = Cast<APRCharacter>(TryGetPawnOwner());
}

void UPRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!IsValid(OwnerCharacter))
    {
        OwnerCharacter = Cast<APRCharacter>(TryGetPawnOwner());
    }
    if (!OwnerCharacter) return;

    FVector Velocity = OwnerCharacter->GetVelocity();

    // 이동 관련
    Speed = Velocity.Size2D();
    ZVelocity = Velocity.Z;
    bIsSprinting = OwnerCharacter->bIsSprinting;
    bIsCrouching = OwnerCharacter->bIsCrouching;
    bIsInAir = OwnerCharacter->bIsInAir;
    bShouldMove = (Speed > 10.f) && !bIsInAir;

    // 전투 관련
    bIsAttacking = OwnerCharacter->bIsAttacking;
    bIsGuarding = OwnerCharacter->bIsGuarding;
    bIsAiming = OwnerCharacter->bIsAiming;

    // 점프 방향 계산
    bJustJumped = OwnerCharacter->bJustJumped;
    MoveDirection = (bIsInAir && bJustJumped)
        ? OwnerCharacter->GetLastJumpDirection()
        : OwnerCharacter->MoveDirection;

    // 무기 타입
    WeaponType = OwnerCharacter->GetCurrentWeaponType();
}


