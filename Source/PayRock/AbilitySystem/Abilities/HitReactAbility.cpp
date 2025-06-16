// PayRockGames


#include "HitReactAbility.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Character/CombatInterface.h"


void UHitReactAbility::ApplyHitReactEffectToOwner()
{
	UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!IsValid(ASC) || !IsValid(HitReactEffect)) return;

	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if (IsValid(AvatarActor))
	{
		if (APRCharacter* PRChar = Cast<APRCharacter>(AvatarActor))
		{
			if (PRChar->IsLocallyControlled())
			{
				PRChar->ShakeCamera();
				PRChar->PlayHitOverlay();
			}
		}
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddHitResult(ASC->StoredHitResult);
	ContextHandle.AddOrigin(GetAvatarActorFromActorInfo()->GetActorLocation());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HitReactEffect, 1.f, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
