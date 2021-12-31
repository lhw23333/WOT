// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
*
*/
UCLASS()
class  UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSetBase();

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health);
	UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Abilities", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities", ReplicatedUsing=OnRep_Armor)
	FGameplayAttributeData	Armor;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Armor);
	UFUNCTION()
	virtual  void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities", ReplicatedUsing=OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Damage);
	UFUNCTION()
	virtual  void OnRep_Damage(const FGameplayAttributeData& OldDamage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities", ReplicatedUsing=OnRep_AP_Count)
	FGameplayAttributeData AP_Count;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, AP_Count);
	UFUNCTION()
	virtual  void OnRep_AP_Count(const FGameplayAttributeData& OldAP_Count);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities", ReplicatedUsing=OnRep_APCR_Count)
	FGameplayAttributeData APCR_Count;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, APCR_Count);
	UFUNCTION()
	virtual  void OnRep_APCR_Count(const FGameplayAttributeData& OldAPCR_Count);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Abilities",ReplicatedUsing= OnRep_HE_Count)
	FGameplayAttributeData HE_Count;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, HE_Count);
	UFUNCTION()
	virtual  void OnRep_HE_Count(const FGameplayAttributeData& OldHE_Count);
};