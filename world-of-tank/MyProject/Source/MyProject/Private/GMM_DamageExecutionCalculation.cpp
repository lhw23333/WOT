// Fill out your copyright notice in the Description page of Project Settings.



#include "GMM_DamageExecutionCalculation.h"
#include "AttributeSetBase.h"
#include "GameplayEffectExecutionCalculation.h"


#include "WeaponAttributeSet.h"

struct SDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	SDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Armor, Target, true);
	}
};

static const SDamageStatics& DamageStatics()
{
	static SDamageStatics DStatics;
	return DStatics;
}

UGMM_DamageExecutionCalculation::UGMM_DamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

float UGMM_DamageExecutionCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = 0.0f;
	GetCapturedAttributeMagnitude(DamageStatics().DamageDef, Spec, EvaluationParameters, Damage);
	
	
	float Armor = 0.0f;
	GetCapturedAttributeMagnitude(DamageStatics().ArmorDef, Spec, EvaluationParameters, Armor);
	//UE_LOG(LogTemp,Warning,TEXT("damage = %f"),Damage);
	return -(Damage - Armor);
}


