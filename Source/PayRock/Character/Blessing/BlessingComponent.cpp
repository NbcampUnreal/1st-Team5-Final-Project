// PayRockGames

#include "BlessingComponent.h"
#include "BlessingDataAsset.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/GameSystem/SaveDataSubsystem.h"

UBlessingComponent::UBlessingComponent() :
	LocalEquippedActiveBlessingIcon(nullptr), OwningPRCharacter(nullptr), CachedAbilitySystemComponent(nullptr),
	InvisibleMaterial(nullptr), InvisibleMaterialDynamic(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBlessingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPRCharacter = Cast<APRCharacter>(GetOwner());
	// Equipped Blessing is saved in the owning client
	if (!OwningPRCharacter->IsLocallyControlled()) return;

	USaveDataSubsystem* SaveDataSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!SaveDataSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveDataSubsystem is null (UBlessingComponent::BeginPlay)"));
		return;
	}
	
	Server_EquipActiveBlessing(SaveDataSystem->GetEquippedActiveBlessing());
	Server_EquipPassiveBlessing(SaveDataSystem->GetEquippedPassiveBlessing());
}

void UBlessingComponent::Server_AddBlessing_Implementation(const FBlessingData& Blessing)
{
	if (Blessing.BlessingName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("(AddBlessing) BlessingName is empty"));
		return;
	}
	AcquiredBlessings.Add(Blessing);

	UE_LOG(LogTemp, Warning, TEXT("Blessing %s added (Server)"), *Blessing.BlessingName.ToString());

	// If listen server, manually call OnRep
	// if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	// {
	// 	if (OwnerCharacter->IsLocallyControlled())
	// 	{
	Client_SaveAddedBlessing(Blessing);
	// }
	// }
}

void UBlessingComponent::Client_SaveAddedBlessing_Implementation(const FBlessingData& Blessing)
{
	if (OwningPRCharacter->IsLocallyControlled())
	{
		if (USaveDataSubsystem* SaveDataSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>())
		{
			SaveDataSystem->SaveBlessing(Blessing);
			UE_LOG(LogTemp, Warning, TEXT("Blessing %s saved to subsystem"),
					*Blessing.BlessingName.ToString());

			Client_BroadcastBlessingAcquired(Blessing);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SaveDataSubsystem is null (OnRep_AcquiredBlessings)"));
		}
	}
}

void UBlessingComponent::Server_EquipActiveBlessing_Implementation(const FBlessingData& Blessing)
{
	if (!GetAbilitySystemComponent()) return;
	if (!IsValid(Blessing.ActiveAbilityClass)) return;

	Server_UnequipActiveBlessing();

	if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(CachedAbilitySystemComponent))
	{
		ActiveBlessingHandle = ASC->AddAbility(
			Blessing.ActiveAbilityClass,
			false,
			FMath::Max(1, static_cast<int>(Blessing.BlessingRarity)));
		EquippedActiveBlessingData = Blessing;
		Client_BroadcastActiveBlessing(Blessing);
	}
}

void UBlessingComponent::Server_EquipPassiveBlessing_Implementation(const FBlessingData& Blessing)
{
	if (!GetAbilitySystemComponent()) return;
	if (IsValid(Blessing.PassiveEffectClass))
	{
		Server_UnequipPassiveBlessing();
	
		FGameplayEffectContextHandle ContextHandle = CachedAbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = CachedAbilitySystemComponent->MakeOutgoingSpec(
			Blessing.PassiveEffectClass,
			FMath::Max(1, static_cast<int>(Blessing.BlessingRarity)),
			ContextHandle);
		PassiveBlessingHandle = CachedAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		EquippedPassiveBlessingData = Blessing;
		Client_BroadcastPassiveBlessing(Blessing);	
	}

	if (IsValid(OwningPRCharacter))
	{
		OwningPRCharacter->InitializeEquipment();
	}
}

void UBlessingComponent::Server_UnequipActiveBlessing_Implementation()
{
	if (!GetAbilitySystemComponent()) return;
	
	if (ActiveBlessingHandle.IsValid())
	{
		CachedAbilitySystemComponent->CancelAbilityHandle(ActiveBlessingHandle);
		CachedAbilitySystemComponent->ClearAbility(ActiveBlessingHandle);
		ActiveBlessingHandle = FGameplayAbilitySpecHandle();
		EquippedActiveBlessingData = FBlessingData();
		// Client_BroadcastActiveBlessing(FBlessingData());
	}
}

void UBlessingComponent::Server_UnequipPassiveBlessing_Implementation()
{
	if (!GetAbilitySystemComponent()) return;
	
	if (PassiveBlessingHandle.IsValid())
	{
		CachedAbilitySystemComponent->RemoveActiveGameplayEffect(PassiveBlessingHandle);
		PassiveBlessingHandle.Invalidate();
		EquippedPassiveBlessingData = FBlessingData();
		// Client_BroadcastPassiveBlessing(FBlessingData());
	}
}

void UBlessingComponent::Client_BroadcastActiveBlessing_Implementation(const FBlessingData& Blessing)
{
	OnActiveBlessingChange.Broadcast(Blessing);

	// client needs to find the texture from savedata subsystem
	if (!GetWorld() || GetWorld()->bIsTearingDown) return;
	USaveDataSubsystem* SaveDataSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!SaveDataSystem) return;

	const UBlessingDataAsset* BlessingDA = SaveDataSystem->GetBlessingDataAsset();
	if (const auto FoundBlessing = BlessingDA->ActiveBlessings.Find(Blessing.BlessingName.ToString()))
	{
		LocalEquippedActiveBlessingIcon = FoundBlessing->Icon;
	}
}

void UBlessingComponent::Client_BroadcastPassiveBlessing_Implementation(const FBlessingData& Blessing)
{
	OnPassiveBlessingChange.Broadcast(Blessing);
}

void UBlessingComponent::Client_BroadcastBlessingAcquired_Implementation(const FBlessingData& Blessing)
{
	OnBlessingAcquired.Broadcast(Blessing);
}

void UBlessingComponent::SetInvisibleMaterial(bool bShouldMakeInvisible)
{
	if (bShouldMakeInvisible)
	{
		if (!InvisibleMaterialDynamic)
		{
			InvisibleMaterialDynamic = UMaterialInstanceDynamic::Create(InvisibleMaterial, OwningPRCharacter);	
		}
		OwningPRCharacter->GetMesh()->SetOverlayMaterial(InvisibleMaterialDynamic);
		OwningPRCharacter->GetWeapon()->SetOverlayMaterial(InvisibleMaterialDynamic);
		OwningPRCharacter->GetWeapon2()->SetOverlayMaterial(InvisibleMaterialDynamic);
	}
	else
	{
		OwningPRCharacter->GetMesh()->SetOverlayMaterial(nullptr);
		OwningPRCharacter->GetWeapon()->SetOverlayMaterial(nullptr);
		OwningPRCharacter->GetWeapon2()->SetOverlayMaterial(nullptr);
	}
	
}

void UBlessingComponent::OnInvisibleTagChanged(const FGameplayTag ChangedTag, int32 TagCount)
{
	if (TagCount > 0)
	{
		OwningPRCharacter->bIsInvisible = true;
		if (OwningPRCharacter->IsLocallyControlled())
		{
			if (!bInvisibleMaterialApplied)
			{
				SetInvisibleMaterial(true);
				bInvisibleMaterialApplied = true;
			}
		}
		else
		{
			OwningPRCharacter->GetMesh()->SetHiddenInGame(true);
			OwningPRCharacter->GetWeapon()->SetHiddenInGame(true);
			OwningPRCharacter->GetWeapon2()->SetHiddenInGame(true);
		}
		Server_UnregisterStimuli();
	}
	else
	{
		OwningPRCharacter->bIsInvisible = false;
		if (OwningPRCharacter->IsLocallyControlled())
		{
			if (bInvisibleMaterialApplied)
			{
				SetInvisibleMaterial(false);
				bInvisibleMaterialApplied = false;
			}
		}
		else
		{
			OwningPRCharacter->GetMesh()->SetHiddenInGame(false);
			OwningPRCharacter->GetWeapon()->SetHiddenInGame(false);
			OwningPRCharacter->GetWeapon2()->SetHiddenInGame(false);
		}
		Server_RegisterStimuli();
	}
}

void UBlessingComponent::Server_UnregisterStimuli_Implementation()
{
	if (!IsValid(OwningPRCharacter->StimuliSourceComponent)) return;
	OwningPRCharacter->StimuliSourceComponent->UnregisterFromPerceptionSystem();
}

void UBlessingComponent::Server_RegisterStimuli_Implementation()
{
	if (!IsValid(OwningPRCharacter->StimuliSourceComponent)) return;
	OwningPRCharacter->StimuliSourceComponent->RegisterWithPerceptionSystem();
}

UAbilitySystemComponent* UBlessingComponent::GetAbilitySystemComponent()
{
	if (CachedAbilitySystemComponent) return CachedAbilitySystemComponent;
	
	if (OwningPRCharacter)
	{
		CachedAbilitySystemComponent = OwningPRCharacter->GetAbilitySystemComponent();
		return CachedAbilitySystemComponent;
	}
	return nullptr;
}
