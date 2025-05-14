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

    if (!OwnerCharacter)
    {
        OwnerCharacter = Cast<APRCharacter>(TryGetPawnOwner());
    }

    if (!OwnerCharacter) return;

    FVector Velocity = OwnerCharacter->GetVelocity();

    // 이동 관련
    Speed = Velocity.Size2D();
    MoveDirection = OwnerCharacter->MoveDirection;
    ZVelocity = Velocity.Z;
    bIsSprinting = OwnerCharacter->bIsSprinting;
    bIsCrouching = OwnerCharacter->bIsCrouching;
    bIsInAir = OwnerCharacter->bIsInAir;
    bShouldMove = (Speed > 10.f) && !bIsInAir;

    // 전투 관련
    bIsAttacking = OwnerCharacter->bIsAttacking;
    bIsGuarding = OwnerCharacter->bIsGuarding;

    // 점프 관련
    bJustJumped = OwnerCharacter->bJustJumped;

    // 점프 중에는 마지막 방향 유지
    const float RawDirection = (bIsInAir && bJustJumped)
        ? OwnerCharacter->GetLastJumpDirection()
        : OwnerCharacter->MoveDirection;
}

