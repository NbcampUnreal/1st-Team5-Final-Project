// PayRockGames


#include "HitReactAbility.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "GameFramework/Actor.h"
#include "PayRock/Character/CombatInterface.h"


void UHitReactAbility::ApplyHitReactEffectToOwner()
{
	UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!IsValid(ASC) || !IsValid(HitReactEffect)) return;

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddHitResult(ASC->StoredHitResult);
	ContextHandle.AddOrigin(GetAvatarActorFromActorInfo()->GetActorLocation());
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HitReactEffect, 1.f, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
