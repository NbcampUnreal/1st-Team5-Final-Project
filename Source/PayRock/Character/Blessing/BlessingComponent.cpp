// PayRockGames

#include "BlessingComponent.h"
#include "BlessingDataAsset.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Character/BaseCharacter.h"

UBlessingComponent::UBlessingComponent() :
	CachedAbilitySystemComponent(nullptr),
	EquippedPassiveBlessingData(nullptr),
	EquippedActiveBlessingData(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBlessingComponent::Server_EquipActiveBlessing_Implementation(UBlessingDataAsset* BlessingDataAsset)
{
	if (!BlessingDataAsset || !GetAbilitySystemComponent()) return;
	if (BlessingDataAsset->ActiveAbilityClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ActiveAbilityClass is not set. Check DA_Blessings"), *BlessingDataAsset->BlessingName.ToString());
		return;
	}

	Server_UnequipActiveBlessing();

	if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(CachedAbilitySystemComponent))
	{
		ActiveBlessingHandle = ASC->AddAbility(
			BlessingDataAsset->ActiveAbilityClass,
			false,
			FMath::Max(1, static_cast<int>(BlessingDataAsset->BlessingRarity)));
		EquippedActiveBlessingData = BlessingDataAsset;
		Client_BroadcastActiveBlessing(BlessingDataAsset);
	}
}

void UBlessingComponent::Server_EquipPassiveBlessing_Implementation(UBlessingDataAsset* BlessingDataAsset)
{
	if (!BlessingDataAsset || !GetAbilitySystemComponent()) return;
	if (BlessingDataAsset->PassiveEffectClass == nullptr)
	{
		const FString BlessingName = BlessingDataAsset ? BlessingDataAsset->BlessingName.ToString() : TEXT("Unknown");
		UE_LOG(LogTemp, Warning, TEXT("%s: PassiveEffectClass is not set. Check DA_Blessings"), *BlessingName);
		return;
	}

	Server_UnequipPassiveBlessing();
	
	FGameplayEffectContextHandle ContextHandle = CachedAbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = CachedAbilitySystemComponent->MakeOutgoingSpec(
		BlessingDataAsset->PassiveEffectClass,
		FMath::Max(1, static_cast<int>(BlessingDataAsset->BlessingRarity)),
		ContextHandle);
	PassiveBlessingHandle = CachedAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	EquippedPassiveBlessingData = BlessingDataAsset;
	Client_BroadcastPassiveBlessing(BlessingDataAsset);
}

void UBlessingComponent::Server_UnequipActiveBlessing_Implementation()
{
	if (!GetAbilitySystemComponent()) return;
	
	if (ActiveBlessingHandle.IsValid())
	{
		CachedAbilitySystemComponent->CancelAbilityHandle(ActiveBlessingHandle);
		CachedAbilitySystemComponent->ClearAbility(ActiveBlessingHandle);
		ActiveBlessingHandle = FGameplayAbilitySpecHandle();
		EquippedActiveBlessingData = nullptr;
		Client_BroadcastActiveBlessing(nullptr);
	}
}

void UBlessingComponent::Server_UnequipPassiveBlessing_Implementation()
{
	if (!GetAbilitySystemComponent()) return;
	
	if (PassiveBlessingHandle.IsValid())
	{
		CachedAbilitySystemComponent->RemoveActiveGameplayEffect(PassiveBlessingHandle);
		PassiveBlessingHandle.Invalidate();
		EquippedPassiveBlessingData = nullptr;
		Client_BroadcastPassiveBlessing(nullptr);
	}
}

void UBlessingComponent::Client_BroadcastActiveBlessing_Implementation(UBlessingDataAsset* BlessingDataAsset)
{
	OnActiveBlessingChange.Broadcast(BlessingDataAsset);
}

void UBlessingComponent::Client_BroadcastPassiveBlessing_Implementation(UBlessingDataAsset* BlessingDataAsset)
{
	OnPassiveBlessingChange.Broadcast(BlessingDataAsset);
}

UAbilitySystemComponent* UBlessingComponent::GetAbilitySystemComponent()
{
	if (CachedAbilitySystemComponent) return CachedAbilitySystemComponent;
	
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		CachedAbilitySystemComponent = Character->GetAbilitySystemComponent();
		return Character->GetAbilitySystemComponent();
	}
	return nullptr;
}
