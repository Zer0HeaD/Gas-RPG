// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RPG_AttributeSet.h"
#include "RPG_GameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/RPG_AbilitySystemLibrary.h"
#include <Interaction/CombatInterface.h>

struct RPGDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)

	RPGDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, CriticalHitDamage, Source, false);
	}
};

static const RPGDamageStatics& DamageStatics()
{
	static RPGDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTag;
	EvaluationParams.TargetTags = TargetTag;

	// Get Damage set By Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FRPG_GameplayTags::Get().Damage);

	// Capture BlockChance on target, and determine if there was a successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().BlockChanceDef, 
		EvaluationParams, 
		TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	// if block, halve the damage.
	Damage = bBlocked ? Damage * 0.5f : Damage;

	// Armor Penetration ignores the percentage of the target's armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorDef,
		EvaluationParams,
		TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	// Add armor penetration modidfier with ArmorPenetrationCoefficient from curve table
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorPenetrationDef,
		EvaluationParams,
		SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = URPG_AbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = 
		CharacterClassInfo->DamageCalculationCieffcients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100;

	// Add EffectiveArmorCoefficient from curve table
	const FRealCurve* EffectiveArmorCurve = 
		CharacterClassInfo->DamageCalculationCieffcients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitChanceDef, 
		EvaluationParams, 
		SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitResistanceDef,
		EvaluationParams,
		TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitDamageDef,
		EvaluationParams,
		SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	// Add EffectiveArmorCoefficient from curve table
	const FRealCurve* CriticalHitResistanceCurve =
		CharacterClassInfo->DamageCalculationCieffcients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	// Critical hit resistance reduces critical hit chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	// Double damage plus a bonus of critical hit
	Damage = bCriticalHit ? Damage * 2.f + SourceCriticalHitDamage : Damage;


	const FGameplayModifierEvaluatedData EvaluatedData(
		URPG_AttributeSet::GetIncomingDamageAttribute(), 
		EGameplayModOp::Additive, 
		Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
