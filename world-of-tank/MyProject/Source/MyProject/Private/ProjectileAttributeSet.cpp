// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAttributeSet.h"

UProjectileAttributeSet::UProjectileAttributeSet()
{
	
}

void UProjectileAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectileAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectileAttributeSet, Count, COND_None, REPNOTIFY_Always);
	
}

void UProjectileAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectileAttributeSet, Damage, OldDamage);
}

void UProjectileAttributeSet::OnRep_Count(const FGameplayAttributeData& OldCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectileAttributeSet, Count, OldCount);
}

