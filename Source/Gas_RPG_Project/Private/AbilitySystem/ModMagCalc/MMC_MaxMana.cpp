// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "AbilitySystem/RPG_AttributeSet.h"
#include <Interaction/CombatInterface.h>

UMMC_MaxMana::UMMC_MaxMana()
{
	MindDef.AttributeToCapture = URPG_AttributeSet::GetMindAttribute();
	MindDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MindDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MindDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags fro source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float intelligence = 0.f;
	GetCapturedAttributeMagnitude(MindDef, Spec, EvaluationParams, intelligence);
	intelligence = FMath::Max<float>(intelligence, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * intelligence + 15.f * PlayerLevel;
}
