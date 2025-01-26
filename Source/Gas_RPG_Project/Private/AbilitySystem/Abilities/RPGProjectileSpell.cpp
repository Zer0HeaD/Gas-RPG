// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/Abilities/RPGProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Interaction/CombatInterface.h>


void URPGProjectileSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URPGProjectileSpell::SpawnProjectile(float ProjectileRotationPitchModifier)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = CombatInterface->GetOwnerRotation();
		//TODO: FOR GRAVITY PROJECTILE SET PITCH TO 25.F LATER
		Rotation.Pitch = ProjectileRotationPitchModifier;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		ARPG_Projectile* Projectile = GetWorld()->SpawnActorDeferred<ARPG_Projectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: give a projectile a gameplay effect spec for causing damage!

		Projectile->FinishSpawning(SpawnTransform);
	}
}
