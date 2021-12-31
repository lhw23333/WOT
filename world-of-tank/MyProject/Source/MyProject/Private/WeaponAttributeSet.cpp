// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAttributeSet.h"


UWeaponAttributeSet::UWeaponAttributeSet()
{
	
}

void UWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, Damage, COND_None, REPNOTIFY_Always);
}

void UWeaponAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, Damage, OldDamage);
}


