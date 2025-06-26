// PayRockGames


#include "SamuraiStatueWeaponAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"

void USamuraiStatueWeaponAbility::OnEventReceived(FGameplayEventData Payload)
{
	Super::OnEventReceived(Payload);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC) return;
	for (AActor* FacingActor : GetActorsFacingAvatar())
	{
		if (UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(FacingActor))
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddOrigin(FacingActor->GetActorLocation());
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
				EffectToApply, GetAbilityLevel(), ContextHandle);
			SourceASC->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				TargetASC
			);	
		}
	}
}

TSet<AActor*> USamuraiStatueWeaponAbility::GetActorsFacingAvatar()
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	if (!SourceActor) return TSet<AActor*>();
	
	FVector Origin = SourceActor->GetActorLocation();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SourceActor);
	QueryParams.bTraceIntoSubComponents = false;
    
	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Origin,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);
	
	TSet<AActor*> FacingActors;
	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Target = Result.GetActor();
		if (!Target || FacingActors.Contains(Target)) continue;
        
		FVector TargetForward = Target->GetActorForwardVector();
		FVector TargetToSource = (Origin - Target->GetActorLocation()).GetSafeNormal();
        
		float DotProduct = FVector::DotProduct(TargetForward, TargetToSource);
		if (DotProduct > FMath::Cos(FMath::DegreesToRadians(AngleThreshold)))
		{
			FacingActors.Add(Target);
		}
	}
    
	return FacingActors;
}