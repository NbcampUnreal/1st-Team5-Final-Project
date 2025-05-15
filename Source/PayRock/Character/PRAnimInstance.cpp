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

    // �̵� ����
    Speed = Velocity.Size2D();
    ZVelocity = Velocity.Z;
    bIsSprinting = OwnerCharacter->bIsSprinting;
    bIsCrouching = OwnerCharacter->bIsCrouching;
    bIsInAir = OwnerCharacter->bIsInAir;
    bShouldMove = (Speed > 10.f) && !bIsInAir;

    // ���� ����
    bIsAttacking = OwnerCharacter->bIsAttacking;
    bIsGuarding = OwnerCharacter->bIsGuarding;
    bIsAiming = OwnerCharacter->bIsAiming;

    // ���� ���� ���
    bJustJumped = OwnerCharacter->bJustJumped;
    MoveDirection = (bIsInAir && bJustJumped)
        ? OwnerCharacter->GetLastJumpDirection()
        : OwnerCharacter->MoveDirection;

    // ���� Ÿ��
    WeaponType = OwnerCharacter->GetCurrentWeaponType();
}


