// PayRockGames

#include "BlessingComponent.h"
#include "BlessingDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Character/BaseCharacter.h"
#include "PayRock/GameSystem/SaveDataSubsystem.h"

UBlessingComponent::UBlessingComponent() :
	CachedAbilitySystemComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBlessingComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBlessingComponent, AcquiredBlessings);
}

void UBlessingComponent::BeginPlay()
{
	Super::BeginPlay();

	// Equipped Blessing is saved in the owning client
	if (!Cast<ACharacter>(GetOwner())->IsLocallyControlled()) return;

	USaveDataSubsystem* SaveDataSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>();
	if (!SaveDataSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveDataSubsystem is null (UBlessingComponent::BeginPlay)"));
		return;
	}
	
	Server_EquipActiveBlessing(SaveDataSystem->GetEquippedActiveBlessing());
	Server_EquipPassiveBlessing(SaveDataSystem->GetEquippedPassiveBlessing());

	//TEST
	Server_EquipActiveBlessing(TestActiveBlessing);
	Server_EquipPassiveBlessing(TestPassiveBlessing);
}

void UBlessingComponent::Server_AddBlessing_Implementation(const FBlessingData& Blessing)
{
	if (Blessing.BlessingName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("(AddBlessing) BlessingName is empty"));
		return;
	}
	AcquiredBlessings.Add(Blessing);

	// If listen server, manually call OnRep
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (OwnerCharacter->IsLocallyControlled())
		{
			OnRep_AcquiredBlessings();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Blessing %s added (Server)"), *Blessing.BlessingName.ToString());
}

void UBlessingComponent::OnRep_AcquiredBlessings()
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (OwnerCharacter->IsLocallyControlled())
		{
			if (USaveDataSubsystem* SaveDataSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveDataSubsystem>())
			{
				if (AcquiredBlessings.Num() > 0)
				{
					FBlessingData LastBlessing = AcquiredBlessings.Last();
					SaveDataSystem->SaveBlessing(LastBlessing);
					
					UE_LOG(LogTemp, Warning, TEXT("Blessing %s saved to subsystem (OnRep)"),
						*LastBlessing.BlessingName.ToString());

					Client_BroadcastBlessingAcquired(LastBlessing);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("SaveDataSubsystem is null (OnRep_AcquiredBlessings)"));
			}
		}
	}
}

void UBlessingComponent::Server_EquipActiveBlessing_Implementation(const FBlessingData& Blessing)
{
	if (!GetAbilitySystemComponent()) return;
	if (Blessing.ActiveAbilityClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blessing %s: ActiveAbilityClass is not set. Check DA_Blessings"), *Blessing.BlessingName.ToString());
		return;
	}

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
	if (Blessing.PassiveEffectClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blessing %s: PassiveEffectClass is not set. Check DA_Blessings"), *Blessing.BlessingName.ToString());
		return;
	}

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
}

void UBlessingComponent::Client_BroadcastPassiveBlessing_Implementation(const FBlessingData& Blessing)
{
	OnPassiveBlessingChange.Broadcast(Blessing);
}

void UBlessingComponent::Client_BroadcastBlessingAcquired_Implementation(const FBlessingData& Blessing)
{
	OnBlessingAcquired.Broadcast(Blessing);
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
