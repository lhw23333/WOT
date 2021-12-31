// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"



UAttributeSetBase::UAttributeSetBase()
{
	
}

void UAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase,Health,COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Damage, COND_None, REPNOTIFY_Always);
	
}

void UAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Health, OldHealth);
}

void UAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UAttributeSetBase::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Armor, OldArmor);
}

void UAttributeSetBase::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Damage,OldDamage);
}

void UAttributeSetBase::OnRep_AP_Count(const FGameplayAttributeData& OldAP_Count)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, AP_Count,OldAP_Count);
}

void UAttributeSetBase::OnRep_APCR_Count(const FGameplayAttributeData& OldAPCR_Count)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, APCR_Count,OldAPCR_Count);
}

void UAttributeSetBase::OnRep_HE_Count(const FGameplayAttributeData& OldHE_Count)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, HE_Count,OldHE_Count);
}





