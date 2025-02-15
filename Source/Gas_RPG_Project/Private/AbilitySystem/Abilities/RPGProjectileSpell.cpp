// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/Abilities/RPGProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Interaction/CombatInterface.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPG_GameplayTags.h"


void URPGProjectileSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URPGProjectileSpell::SpawnProjectile(float ProjectileRotationPitchModifier, bool bUsePitchModifier)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = CombatInterface->GetOwnerRotation();
		if (bUsePitchModifier)
		{
			Rotation.Pitch = ProjectileRotationPitchModifier;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		ARPG_Projectile* Projectile = GetWorld()->SpawnActorDeferred<ARPG_Projectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* Source_ASC = 
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		check(DamageEffectClass);
		const FGameplayEffectSpecHandle SpecHandle =
			Source_ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), Source_ASC->MakeEffectContext());

		FRPG_GameplayTags GameplayTags = FRPG_GameplayTags::Get();

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
