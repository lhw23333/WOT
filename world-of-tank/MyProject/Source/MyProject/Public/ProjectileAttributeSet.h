// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectileAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MYPROJECT_API UProjectileAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UProjectileAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",Replicated =OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UProjectileAttributeSet, Damage);

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldDamage);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities",Replicated =OnRep_Count)
	FGameplayAttributeData Count;
	ATTRIBUTE_ACCESSORS(UProjectileAttributeSet, Count);

	UFUNCTION()
	virtual void OnRep_Count(const FGameplayAttributeData& OldCount);
};
