// PayRockGames

#include "PRAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/Character/CombatInterface.h"

UPRAttributeSet::UPRAttributeSet()
{
}

void UPRAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* Primary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Spirit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Luck, COND_None, REPNOTIFY_Always);

	/* Secondary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CriticalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, DebuffResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, LootQualityModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CarryWeight, COND_None, REPNOTIFY_Always);
	
	/* Vital Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

/*
void UPRAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}
*/

void UPRAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid()
		&& Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && IsValid(Props.SourceAvatarActor))
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UPRAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	//if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props, Data);
	}
}

void UPRAttributeSet::HandleIncomingDamage(const FEffectProperties& Props, const FGameplayEffectModCallbackData& Data)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		float NewHealth = FMath::Clamp(GetHealth() - LocalIncomingDamage, 0.f, GetMaxHealth());

		FOnAttributeChangeData AttributeChangeData;
		AttributeChangeData.Attribute = GetHealthAttribute();
		AttributeChangeData.GEModData = &Data;
		AttributeChangeData.OldValue = GetHealth();
		AttributeChangeData.NewValue = NewHealth;
		
		//SetHealth(NewHealth);
		GetHealthAttribute().SetNumericValueChecked(NewHealth, this);
		
		GetOwningAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
			GetHealthAttribute()).Broadcast(AttributeChangeData);
		
		UE_LOG(LogTemp, Warning, TEXT("IncomingDamage: %f / Health(Current): %f / GetHealth(Base): %f"),
			LocalIncomingDamage, GetHealth(), GetOwningAbilitySystemComponent()->GetNumericAttributeBase(GetHealthAttribute()));

		const bool bShouldDie = NewHealth <= 0.f;
		if (NewHealth <= 0.f)
		{
			// Handle death
		}
		else
		{
			// Handle knockback, status effects, etc.
		}
	}
}

/* Primary Attributes */

void UPRAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Strength, OldStrength);
}

void UPRAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Intelligence, OldIntelligence);
}

void UPRAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Dexterity, OldDexterity);
}

void UPRAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Vitality, OldVitality);
}

void UPRAttributeSet::OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Spirit, OldSpirit);
}

void UPRAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Luck, OldLuck);
}

/* Secondary Attributes */

void UPRAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Armor, OldArmor);
}

void UPRAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, BlockChance, OldBlockChance);
}

void UPRAttributeSet::OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CriticalResistance, OldCriticalResistance);
}

void UPRAttributeSet::OnRep_DebuffResistance(const FGameplayAttributeData& OldDebuffResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, DebuffResistance, OldDebuffResistance);
}

void UPRAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, MaxHealth, OldMaxHealth);
}

void UPRAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, MaxMana, OldMaxMana);
}

void UPRAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UPRAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UPRAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UPRAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CriticalChance, OldCriticalChance);
}

void UPRAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CriticalDamage, OldCriticalDamage);
}

void UPRAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UPRAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UPRAttributeSet::OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CooldownReduction, OldCooldownReduction);
}

void UPRAttributeSet::OnRep_LootQualityModifier(const FGameplayAttributeData& OldLootQualityModifier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, LootQualityModifier, OldLootQualityModifier);
}

void UPRAttributeSet::OnRep_CarryWeight(const FGameplayAttributeData& OldCarryWeight) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CarryWeight, OldCarryWeight)
}

/* Vital Attributes */

void UPRAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Health, OldHealth);
}

void UPRAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Mana, OldMana);	
}

