// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Actor/WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/RPG_Player_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/RPG_ParkourMovementComponent.h"
//#include "StoredData.h"
#include "RecoilAnimationComponent.h"
#include "GameFramework/PlayerController.h"

#include "DrawDebugHelpers.h"
#include <random>

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// SOCKET_Weapon
	// SOCKET_Back_Weapon
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	PlayerOwner = Cast<ARPG_Player_Character>(GetOwner());
	if (PlayerOwner)
	{
		PlayerController = Cast<APlayerController>(PlayerOwner->GetController());
		AnimInstance = PlayerOwner->GetMesh()->GetAnimInstance();
		PlayerRecoilAnimComponent = PlayerOwner->RecoilAnimationComponent;
	}

	// INITIALIZE EQUIPMENT AMMO DATA
	AmmoMap.Add(EAmmoType::Ammo_AR, FAmmoData(120, 180));
	AmmoMap.Add(EAmmoType::Ammo_SMG, FAmmoData(140, 210));
	AmmoMap.Add(EAmmoType::Ammo_Pistol, FAmmoData(45, 90));
	AmmoMap.Add(EAmmoType::Ammo_Shotgun, FAmmoData(24, 48));

	// Spawn a startupGun
	FActorSpawnParameters spawnParams;

	if (!WeaponsArray.IsEmpty())
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponsArray[0], spawnParams);
		CurrentWeaponIndex = 0;
		if (PlayerOwner)
		{
			InitializeCurrentWeaponVariables();
			PlayerOwner->CreateWeaponBar();
			UnholsterWeapon();
		}
	}
}

void UCombatComponent::InitializeCurrentWeaponVariables()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeaponSettings = CurrentWeapon->WeaponSettings;

		PlayerRecoilAnimComponent->Init(CurrentWeaponSettings.RecoilAnimData,
			CurrentWeaponSettings.FireRateAnimation, CurrentWeaponSettings.Burst);

		PlayerRecoilAnimComponent->SetFireMode(CurrentWeaponSettings.FireMode);

		CurrentWeaponAnimInstance = CurrentWeapon->WeaponMesh->GetAnimInstance();

		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WEAPON SETTINGS SUCCESSFULLY UPDATED!"), true, false, FColor::Orange, 10.f);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ERROR! FAILED TO UPDATE WEAPON SETTINGS!"), true, false, FColor::Red, 10.f);
	}
}

void UCombatComponent::StartFiring()
{
	if (CanFire())
	{
		if (CurrentWeaponSettings.IsPumpOrBoltAction) { GetWorld()->GetTimerManager().ClearTimer(PumpOrBoltReloadTimerHandle); }

		switch (CurrentWeaponSettings.FireMode)
		{
		case EFireMode_PRAS::Semi:

			if (bUnlockFire)
			{
				bFiringAWeapon = true;
				PlayerState = EPlayerStates::Shooting;
				OnFire();

				GetWorld()->GetTimerManager().SetTimer(
					LockingFireHandle,
					this,
					&UCombatComponent::UnlockFire,
					CurrentWeaponSettings.FireRateTimer,
					false);
				bUnlockFire = false;
			}
			else
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Wait for unlock semi fire!"), true, false, FColor::Orange, 2.f);
			}

			break;
		case EFireMode_PRAS::Auto:

			/*bFiringAWeapon = true;
			PlayerState = EPlayerStates::Shooting;
			OnFire();
			GetWorld()->GetTimerManager().SetTimer(
				AutomaticFireHandle,
				this,
				&UCombatComponent::OnFire,
				CurrentWeaponSettings.FireRateTimer,
				true);*/
			if (bUnlockFire)
			{
				bFiringAWeapon = true;
				PlayerState = EPlayerStates::Shooting;
				OnFire();

				GetWorld()->GetTimerManager().SetTimer(
					LockingFireHandle,
					this,
					&UCombatComponent::UnlockFire,
					CurrentWeaponSettings.FireRateTimer,
					false);
				bUnlockFire = false;
			}
			else
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Wait for unlock semi fire!"), true, false, FColor::Orange, 2.f);
			}

			break;
		case EFireMode_PRAS::Burst:

			if (bUnlockFire)
			{
				if (BurstCurrentShots < CurrentWeaponSettings.Burst)
				{
					bFiringAWeapon = true;
					PlayerState = EPlayerStates::Shooting;

					GetWorld()->GetTimerManager().SetTimer(
						BurstFireHandle,
						this,
						&UCombatComponent::StartFiring,
						CurrentWeaponSettings.FireRateTimer,
						true);

					++BurstCurrentShots;
					OnFire();
				}
				else
				{
					bFiringAWeapon = false;
					GetWorld()->GetTimerManager().ClearTimer(BurstFireHandle);
					BurstCurrentShots = 0;
					PlayerRecoilAnimComponent->Stop();

					if (PlayerState == EPlayerStates::Shooting)
					{
						PlayerState = EPlayerStates::Unoccupied;
					}

					bUnlockFire = false;
					GetWorld()->GetTimerManager().SetTimer(
						LockingFireHandle,
						this,
						&UCombatComponent::UnlockFire,
						BurstLockingTimer,
						false);
				}
			}
			else
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Wait for unlock burst fire!"), true, false, FColor::Orange, 2.f);
			}

			break;
		default:
			break;
		}
	}
}

void UCombatComponent::StopFiring()
{
	bFiringAWeapon = false;
	PlayerRecoilAnimComponent->Stop();
	if (PlayerState == EPlayerStates::Shooting)
	{
		PlayerState = EPlayerStates::Unoccupied;
	}

	switch (CurrentWeaponSettings.FireMode)
	{
	case EFireMode_PRAS::Semi:

		break;

	case EFireMode_PRAS::Auto:

		GetWorld()->GetTimerManager().ClearTimer(AutomaticFireHandle);
		break;

	case EFireMode_PRAS::Burst:

		break;
	default:
		break;
	}
}

bool UCombatComponent::CanFire()
{
	bool IsBurstShootingInProgress =	PlayerState == EPlayerStates::Shooting && 
										CurrentWeaponSettings.FireMode == EFireMode_PRAS::Burst &&
										CurrentWeaponSettings.CurrentMagazineAmmo > 0;


	if (IsBurstShootingInProgress || 
		(CurrentWeapon && CurrentWeaponSettings.CurrentMagazineAmmo > 0 && PlayerState == EPlayerStates::Unoccupied))
	{
		return true;
	}
	return false;
}

void UCombatComponent::UnlockFire()
{
	bUnlockFire = true;
}

void UCombatComponent::StartADS()
{
	bInADS = true;
	ADSAlpha = 1.f;

	PlayerRecoilAnimComponent->SetAimingStatus(true);
	PlayerOwner->OnAimStarted();

	if (CurrentWeaponSettings.AimInAnimation != nullptr && PlayerState == EPlayerStates::Unoccupied)
	{
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(CurrentWeaponSettings.AimInAnimation, 1.f);
		}
	}

	if (CurrentWeaponSettings.AimInSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CurrentWeaponSettings.AimInSound, PlayerOwner->GetActorLocation());
	}
}

void UCombatComponent::StopADS()
{
	bInADS = false;

	// TODO: not sure if it needs, but make sense to interpolate ADSAlpha (control recoil and check if we in ADS or not)
	ADSAlpha = 0.f;

	CurrentADSHeat = 0.f;

	PlayerRecoilAnimComponent->SetAimingStatus(false);
	PlayerOwner->OnAimEnded();

	if (CurrentWeaponSettings.AimOutAnimation != nullptr && PlayerState == EPlayerStates::Unoccupied)
	{
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(CurrentWeaponSettings.AimOutAnimation, 1.f);
		}
	}

	if (CurrentWeaponSettings.AimOutSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CurrentWeaponSettings.AimOutSound, PlayerOwner->GetActorLocation());
	}
}

void UCombatComponent::SwapWeapons(int32 NewWeaponIndex)
{
	if (WeaponsArray.Num() >= 2 && WeaponsArray.IsValidIndex(NewWeaponIndex) && CurrentWeaponIndex != NewWeaponIndex)
	{
		bIsSwitchingWeapons = true;
		SavedNewWeaponIndex = NewWeaponIndex;
		CurrentWeaponIndex = SavedNewWeaponIndex;
		// Holster Current Weapon
		HolsterWeapon();
	}
	else
	{
		UKismetSystemLibrary::PrintString(
			GetWorld(), 
			TEXT("WARNING: FAILED TO SWAP A GUNS. INDEX OUT OF BOUNDS OR WEAPONS ARRAY CONTAINS ONLY ONE GUN!"), 
			true, 
			false, 
			FColor::Red, 
			5.f);
	}
}

void UCombatComponent::ShootAProjectile(const UWorld* World, 
	FVector MuzzleSocketLocation, FHitResult result, FActorSpawnParameters ActorSpawnParams, FVector LaunchDirection)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerOwner);

	FVector TraceStart = MuzzleSocketLocation;
	FVector ShotDirection = CameraRotation.Vector();

	// ADD BULLET SPREAD
	CurrentBloom = FMath::Clamp(CurrentBloom + CurrentWeaponSettings.BloomStep, 0.f, CurrentWeaponSettings.MaxBloom);
	float spread = CurrentBloom;
	float bloomModifier = FMath::Lerp(1.f, CurrentWeaponSettings.ADSBloomModifier, ADSAlpha);
	spread *= bloomModifier;

	ShotDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ShotDirection, spread);

	FVector TraceEnd = TraceStart + ShotDirection * 10000;

	// CALCULATE SHOT DIRECTION
	bool bHasHit = World->LineTraceSingleByChannel(result, TraceStart, TraceEnd, ECC_Visibility, Params);

	if (bDebug)
	{
		// DEBUG: Draw the spread cone
		float ConeLength = 1000.0f;  // Adjust the length of the cone to visualize spread
		DrawDebugCone(World, TraceStart, CameraRotation.Vector(), ConeLength,
			FMath::DegreesToRadians(spread), FMath::DegreesToRadians(spread), 12, FColor::Green, false, 2.0f, 0, 1.0f);

		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 0.5f);
		if (bHasHit)
		{
			DrawDebugBox(World, result.Location, FVector(15), FColor::Green, false, 3.0f, 0, 3.0f);
		}
	}

	// LAUNCH A PROJECTILE
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	LaunchDirection = (TraceEnd - TraceStart).GetSafeNormal();

	// spawn a bullet
	if (CurrentWeaponSettings.BulletProjectile != NULL)
	{
		// BROADCAST SPAWN AN ABILITY SYSTEM PROJECTILE
		ShootProjectileDelegate.Broadcast(
			CurrentWeaponSettings.BulletProjectile, 
			MuzzleSocketLocation, 
			LaunchDirection.Rotation());
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING! No projectile to launch!"), true, false, FColor::Red, 5.f);
	}

	//  We want to spawn a hit decal of every shot
	/*if (CurrentWeaponSettings.BulletHoleDecal != nullptr)
	{
		UGameplayStatics::SpawnDecalAtLocation(World,
			CurrentWeaponSettings.BulletHoleDecal, FVector(15.f), result.Location, result.ImpactNormal.Rotation(), 30.f);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING! No BulletHoleDecal to spawn!"), true, false, FColor::Red, 5.f);
	}*/
}

void UCombatComponent::OnFire()
{
	if (PlayerState != EPlayerStates::Shooting) { return; }

	const UWorld* World = GetWorld();
	if (CurrentWeapon == nullptr)
	{
		UKismetSystemLibrary::PrintString(World, TEXT("WEAPON NOT VALID FOR MAKE A SHOT!"));
		return;
	}

	FVector MuzzleSocketLocation = CurrentWeapon->WeaponMesh->GetSocketLocation(FName("SOCKET_Muzzle"));
	FVector EjectSocketLocation = CurrentWeapon->WeaponMesh->GetSocketLocation(FName("SOCKET_Eject"));
	FHitResult result;
	FVector LaunchDirection = FVector::ZeroVector;
	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;

	if (World != nullptr)
	{
		if (CurrentWeaponSettings.CurrentMagazineAmmo > 0)
		{
			CurrentWeaponSettings.CurrentMagazineAmmo--;

			for (int i = 0; i < CurrentWeaponSettings.NumberOfBulletsPerShot; ++i)
			{
				ShootAProjectile(World, MuzzleSocketLocation, result, ActorSpawnParams, LaunchDirection);
			}

			// spawn a Shell eject
			if (CurrentWeaponSettings.BulletShellEject != NULL)
			{
				GetWorld()->SpawnActor<AActor>(
					CurrentWeaponSettings.BulletShellEject,
					EjectSocketLocation,
					LaunchDirection.Rotation(),
					ActorSpawnParams);
			}
			else
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING! No shellEject to launch!"), true, false, FColor::Red, 5.f);
			}

			// Start Input Recoil
			if (bIsRecoilNeutral)
			{
				RecoilCheckpoint = PlayerOwner->GetControlRotation();
				bIsRecoilNeutral = false;
			}
			if (bUpdateRecoilPitchCheckpointInNextShot)
			{
				RecoilCheckpoint = FRotator(PlayerOwner->GetControlRotation().Pitch, RecoilCheckpoint.Yaw, RecoilCheckpoint.Roll);
				bUpdateRecoilPitchCheckpointInNextShot = false;
			}
			if (bUpdateRecoilYawCheckpointInNextShot)
			{
				RecoilCheckpoint = FRotator(RecoilCheckpoint.Pitch, PlayerOwner->GetControlRotation().Yaw, RecoilCheckpoint.Roll);
				bUpdateRecoilYawCheckpointInNextShot = false;
			}
			StartRecoil();

			// Start Procedural Recoil
			if (!bInADS)
			{
				PlayerRecoilAnimComponent->Play();
			}

			// Start Camera Shake
			if (CurrentWeaponSettings.RecoilCameraShake != NULL)
			{
				UGameplayStatics::GetPlayerCameraManager(PlayerOwner, 0)->PlayWorldCameraShake(
					GetWorld(),
					CurrentWeaponSettings.RecoilCameraShake,
					PlayerOwner->GetActorLocation(),
					0.f,
					500.f,
					1.f);
			}

			// COSMETIC 

			// updating weapon bar in blueprints
			PlayerOwner->UpdateWeaponBarCurrentAmmo();

			if (CurrentWeaponSettings.WeaponMuzzleParticle != nullptr)
			{
				UGameplayStatics::SpawnEmitterAttached(
					CurrentWeaponSettings.WeaponMuzzleParticle, // Particle system to spawn
					CurrentWeapon->WeaponMesh,                                        // Attach to the weapon's mesh
					FName("SOCKET_Muzzle"),                                  // Attach to the muzzle socket
					FVector::ZeroVector,                               // Location offset (relative to socket)
					CurrentWeaponSettings.WeaponMuzzleParticleRotation,      // Rotation offset (relative to socket)
					EAttachLocation::SnapToTarget,                     // Attach location type
					true                                               // Auto-destroy when done
				);
			}

			/*if (CurrentWeaponSettings.HitEffectParticle != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, CurrentWeaponSettings.HitEffectParticle, result.Location);
			}*/

			// Try and play the sound if specified
			if (CurrentWeaponSettings.FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, CurrentWeaponSettings.FireSound, PlayerOwner->GetActorLocation());
			}

			// Try and play a firing animation if specified
			if (bInADS)
			{
				if (CurrentWeaponSettings.FireAimAnimation != nullptr)
				{
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(CurrentWeaponSettings.FireAimAnimation, 1.f);
					}
				}
			}
			else
			{
				if (CurrentWeaponSettings.FireAnimation != nullptr)
				{
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(CurrentWeaponSettings.FireAnimation, 1.f);
					}
				}
			}

			// weapon's fire animation
			if (CurrentWeaponSettings.WeaponFireAnimation != nullptr)
			{
				if (CurrentWeaponAnimInstance != nullptr)
				{
					CurrentWeaponAnimInstance->Montage_Play(CurrentWeaponSettings.WeaponFireAnimation, 1.f);
				}
			}
		}
		else
		{
			StopFiring();
			UKismetSystemLibrary::PrintString(World, TEXT("Out of ammo!"));

			if (bInADS)
			{
				if (CurrentWeaponSettings.EmptyFireAimAnimation != nullptr)
				{
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(CurrentWeaponSettings.EmptyFireAimAnimation, 1.f);
					}
				}
			}
			else
			{
				if (CurrentWeaponSettings.EmptyFireAnimation != nullptr)
				{
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(CurrentWeaponSettings.EmptyFireAnimation, 1.f);
					}
				}
			}

			if (CurrentWeaponSettings.FireEmptySound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, CurrentWeaponSettings.FireEmptySound, PlayerOwner->GetActorLocation());
			}
		}
	}
	//PlayerState = EPlayerStates::Unoccupied;
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRecoilActive)
	{
		if (PlayerOwner)
		{
			PlayerOwner->AddControllerPitchInput((RecoilPitchVelocity * DeltaTime) * -1.f);
			PlayerOwner->AddControllerYawInput(RecoilYawVelocity * DeltaTime);

			RecoilPitchVelocity -= RecoilPitchDamping * DeltaTime;
			RecoilYawVelocity -= RecoilYawDamping * DeltaTime;

			if (RecoilPitchVelocity <= 0.f)
			{
				bIsRecoilActive = false;
				StartRecoilRecovery();
			}
		}
	}
	else if (bIsRecoilRecoveryActive)
	{
		if (PlayerOwner && PlayerController)
		{
			FRotator currentControlRotation = PlayerOwner->GetControlRotation();

			FRotator deltaRotation = currentControlRotation - RecoilCheckpoint;
			deltaRotation.Normalize();

			if (FMath::Abs(deltaRotation.Pitch) > 1.f)
			{
				float interpSpeed = (1.f / DeltaTime) / 4.f;

				FRotator interpRotation = FMath::RInterpConstantTo(currentControlRotation, RecoilCheckpoint, DeltaTime, interpSpeed);
				interpSpeed = (1.f / DeltaTime) / 10.f;
				interpRotation.Yaw = FMath::RInterpTo(currentControlRotation, RecoilCheckpoint, DeltaTime, interpSpeed).Yaw;
				if (!bIsRecoilYawRecoveryActive)
				{
					interpRotation.Yaw = currentControlRotation.Yaw;
				}

				PlayerController->SetControlRotation(interpRotation);
			}
			else if (FMath::Abs(deltaRotation.Pitch) > 0.1f)
			{
				float interpSpeed = (1.f / DeltaTime) / 6.f;

				FRotator interpRotation = FMath::RInterpTo(currentControlRotation, RecoilCheckpoint, DeltaTime, interpSpeed);

				if (!bIsRecoilYawRecoveryActive)
				{
					interpRotation.Yaw = currentControlRotation.Yaw;
				}

				PlayerController->SetControlRotation(interpRotation);
			}
			else
			{
				bIsRecoilRecoveryActive = false;
				bIsRecoilYawRecoveryActive = false;
				bIsRecoilNeutral = true;
			}
		}
	}

	if (CurrentBloom > 0.f)
	{
		float interpSpeed = (1.f / DeltaTime) / CurrentWeaponSettings.BloomRecoveryInterpSpeed;

		CurrentBloom = FMath::FInterpConstantTo(CurrentBloom, 0.f, DeltaTime, interpSpeed);
	}
}

void UCombatComponent::UnholsterWeapon()
{
	if (!IsPlayerUnnocupied()) return;
	PlayerState = EPlayerStates::GettingReady;

	AttachCurrentWeaponToSocket(FName("SOCKET_Weapon"));
	if (AnimInstance)
	{
		float currentAnimLength = 0.5f;
		if (CurrentWeaponSettings.UnholsterAnimation)
		{
			AnimInstance->Montage_Play(CurrentWeaponSettings.UnholsterAnimation);
			currentAnimLength = AnimInstance->GetCurrentActiveMontage()->GetPlayLength();
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: UnholsterAnimation IS NULL!"), true, false, FColor::Red, 5.f);
		}

		if (CurrentWeaponSettings.WeaponUnholsterAnimation)
		{
			PlayWeaponAnimation(CurrentWeaponSettings.WeaponUnholsterAnimation);
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: Weapon UnholsterAnimation IS NULL!"), true, false, FColor::Red, 5.f);
		}
		
		if (CurrentWeaponSettings.UnholsterSound)
		{
			PlaySoundAtOwner(CurrentWeaponSettings.UnholsterSound);
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: UnholsterSound IS NULL!"), true, false, FColor::Red, 5.f);
		}

		GetWorld()->GetTimerManager().SetTimer(
			UnholsterHandle,
			this,
			&UCombatComponent::HandleUnholstering,
			currentAnimLength - CurrentWeaponSettings.UnholsterAnimationEarlyUnlockModifier,
			false);
	}
}

void UCombatComponent::QuickUnholsterWeapon()
{
	if (!IsPlayerUnnocupied()) return;
	PlayerState = EPlayerStates::GettingReady;

	AttachCurrentWeaponToSocket(FName("SOCKET_Weapon"));

	if (AnimInstance)
	{
		// Stop prev monatge
		AnimInstance->StopAllMontages(0.05f);

		float currentAnimLength = 0.5f;
		if (CurrentWeaponSettings.UnholsterQuickAnimation)
		{
			AnimInstance->Montage_Play(CurrentWeaponSettings.UnholsterQuickAnimation);
			currentAnimLength = AnimInstance->GetCurrentActiveMontage()->GetPlayLength();
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: UnholsterQuickAnimation IS NULL!"), true, false, FColor::Red, 5.f);
		}

		if (CurrentWeaponSettings.UnholsterSound)
		{
			PlaySoundAtOwner(CurrentWeaponSettings.UnholsterSound);
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: UnholsterSound IS NULL!"), true, false, FColor::Red, 5.f);
		}

		GetWorld()->GetTimerManager().SetTimer(
			UnholsterHandle,
			this,
			&UCombatComponent::HandleUnholstering,
			currentAnimLength,
			false);
	}
}

void UCombatComponent::HolsterWeapon()
{
	if (!IsPlayerUnnocupied()) return;
	PlayerState = EPlayerStates::GettingReady;
	if (AnimInstance)
	{
		float currentAnimLength = 0.5f;
		if (CurrentWeaponSettings.UnholsterAnimation)
		{
			AnimInstance->Montage_Play(CurrentWeaponSettings.HolsterAnimation);
			currentAnimLength = AnimInstance->GetCurrentActiveMontage()->GetPlayLength();
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: HolsterAnimation IS NULL!"), true, false, FColor::Red, 5.f);
		}

		//PlayWeaponAnimation(CurrentWeaponSettings.WeaponHolsterAnimation);
		if (CurrentWeaponSettings.HolsterSound)
		{
			PlaySoundAtOwner(CurrentWeaponSettings.HolsterSound);
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: HolsterSound IS NULL!"), true, false, FColor::Red, 5.f);
		}

		GetWorld()->GetTimerManager().SetTimer(
			HolsterHandle,
			this,
			&UCombatComponent::HandleHolstering,
			currentAnimLength,
			false);
	}
}

void UCombatComponent::HandleUnholstering()
{
	PlayerState = EPlayerStates::Unoccupied;
	if (bIsSwitchingWeapons)
	{
		bIsSwitchingWeapons = false;
	}
}

void UCombatComponent::HandleHolstering()
{
	PlayerState = EPlayerStates::Unoccupied;
	// Attach execute in custom anim notify, in animation
	//AttachCurrentWeaponToSocket(FName("SOCKET_Back_Weapon"));
	if (bIsSwitchingWeapons)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
		}

		FActorSpawnParameters spawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponsArray[SavedNewWeaponIndex], spawnParams);
		if (PlayerOwner)
		{
			InitializeCurrentWeaponVariables();
			PlayerOwner->UpdateWeaponBarMaxAmmo();
		}
		UnholsterWeapon();
		SavedNewWeaponIndex = 0;
	}
}

void UCombatComponent::StartRecoilRecovery()
{
	bIsRecoilRecoveryActive = true;
	bIsRecoilYawRecoveryActive = true;
}

void UCombatComponent::StartRecoil()
{
	//RecoilTimeline.PlayFromStart(); // OLD

	InitialRecoilPitchForce = CurrentWeaponSettings.BaseRecoilPitchForce;
	InitialRecoilYawForce = CurrentWeaponSettings.BaseRecoilYawForce;

	if (CurrentWeaponSettings.FireMode == EFireMode_PRAS::Auto)
	{
		CurrentADSHeat = ADSAlpha > 0.f ? CurrentADSHeat + 1.f : 0.f;

		float ADSHeatModifier = FMath::Clamp(CurrentADSHeat / CurrentWeaponSettings.MaxADSHeat, 0.f, CurrentWeaponSettings.ADSheatModifierMax);
		InitialRecoilPitchForce *= 1.f - ADSHeatModifier;
		InitialRecoilYawForce *= 1.f - ADSHeatModifier;
	}

	RecoilPitchVelocity = InitialRecoilPitchForce;
	RecoilPitchDamping = RecoilPitchVelocity / 0.1f;

	std::random_device rd;
	std::mt19937 gen(rd());
	float directionStat = RecoilDirectionCurve->GetFloatValue(CurrentWeaponSettings.RecoilStat);
	float directionScaleModifier = directionStat / 100.f;
	float stddev = InitialRecoilYawForce * (1.f - CurrentWeaponSettings.RecoilStat / 100.f);

	std::normal_distribution<float> d(InitialRecoilYawForce * directionScaleModifier, stddev);
	RecoilYawVelocity = d(gen);
	RecoilYawDamping = (RecoilYawVelocity * -1.f) / 0.1f;

	bIsRecoilActive = true;
}

void UCombatComponent::EndRecoil()
{

}

// on pick up ammo and add it to stock
void UCombatComponent::AddAmmo(EAmmoType AmmoType, int32 AmountToAdd)
{
	if (FAmmoData* AmmoData = AmmoMap.Find(AmmoType))
	{
		AmmoData->CurrentAmmo = FMath::Clamp(AmmoData->CurrentAmmo + AmountToAdd, 0, AmmoData->MaxAmmo);
	}
}

// CURRENTLY NOT IN USE!
bool UCombatComponent::ConsumeAmmo(EAmmoType AmmoType, int32 AmountConsume)
{
	if (FAmmoData* AmmoData = AmmoMap.Find(AmmoType))
	{
		if (AmmoData->CurrentAmmo >= AmountConsume)
		{
			AmmoData->CurrentAmmo -= AmountConsume;
			return true;
		}
	}
	return false;
}

void UCombatComponent::ReloadWeapon(EAmmoType AmmoType)
{
	if (FAmmoData* AmmoData = AmmoMap.Find(AmmoType))
	{
		if (AmmoData->CurrentAmmo > 0)
		{
			if (CanReload())
			{
				PlayerState = EPlayerStates::Reloading;
				if (bInADS)
				{
					// regular  aim reload
					if (CurrentWeaponSettings.CurrentMagazineAmmo > 0)
					{
						PlaySoundAtOwner(CurrentWeaponSettings.ReloadSound);
						// Try and play a reload animation if specified
						PlayAnimationWithDelay(CurrentWeaponSettings.ReloadAimAnimation);
						// Try and play a weapon animation
						PlayWeaponAnimation(CurrentWeaponSettings.WeaponReloadAnimation);
					}
					// empty aim reload
					else
					{
						PlaySoundAtOwner(CurrentWeaponSettings.ReloadEmptySound);
						// Try and play a reload animation if specified
						PlayAnimationWithDelay(CurrentWeaponSettings.ReloadAimEmptyAnimation);
						// Try and play a weapon animation
						PlayWeaponAnimation(CurrentWeaponSettings.WeaponEmptyReloadAnimation);
					}
				}
				else
				{
					// regular reload
					if (CurrentWeaponSettings.CurrentMagazineAmmo > 0)
					{
						PlaySoundAtOwner(CurrentWeaponSettings.ReloadSound);
						// Try and play a reload animation if specified
						PlayAnimationWithDelay(CurrentWeaponSettings.ReloadAnimation);
						// Try and play a weapon animation
						PlayWeaponAnimation(CurrentWeaponSettings.WeaponReloadAnimation);
					}
					// empty reload
					else
					{
						PlaySoundAtOwner(CurrentWeaponSettings.ReloadEmptySound);
						// Try and play a reload animation if specified
						PlayAnimationWithDelay(CurrentWeaponSettings.ReloadEmptyAnimation);
						// Try and play a weapon animation
						PlayWeaponAnimation(CurrentWeaponSettings.WeaponEmptyReloadAnimation);
					}
				}
			}
			else
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Reload in process..."), true, false, FColor::Orange, 3.f);
			}
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NO AMMO OF THIS TYPE IN STOCK! RELOAD CANCELED!"), true, false, FColor::Orange, 5.f);
		}
	}
}

void UCombatComponent::HandleReload(EAmmoType AmmoType)
{
	FAmmoData* AmmoData = AmmoMap.Find(AmmoType);

	int32 AmmoNeeded = CurrentWeaponSettings.MagazineCapacity - CurrentWeaponSettings.CurrentMagazineAmmo;

	// Check if there's enough ammo in the inventory to reload
	if (AmmoData->CurrentAmmo >= AmmoNeeded)
	{
		// Reload the magazine fully
		CurrentWeaponSettings.CurrentMagazineAmmo += AmmoNeeded;
		AmmoData->CurrentAmmo -= AmmoNeeded;
	}
	else
	{
		// Reload with whatever ammo is left
		CurrentWeaponSettings.CurrentMagazineAmmo += AmmoData->CurrentAmmo;
		AmmoData->CurrentAmmo = 0;
	}
}

void UCombatComponent::HandlePumpOrBoltWeaponReload(EAmmoType AmmoType)
{
	if (FAmmoData* AmmoData = AmmoMap.Find(AmmoType))
	{
		if (AmmoData->CurrentAmmo <= 0 || CurrentWeaponSettings.CurrentMagazineAmmo >= CurrentWeaponSettings.MagazineCapacity)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Finish reloading!"), true, false, FColor::Orange, 3.f);
			PlaySoundAtOwner(CurrentWeaponSettings.ReloadEndSound);
			if (CurrentWeaponSettings.ReloadEndAnimation != nullptr)
			{
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(CurrentWeaponSettings.ReloadEndAnimation, 1.f);
				}
			}
			PlayWeaponAnimation(CurrentWeaponSettings.WeaponReloadEndAnimation);
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No Current ammo or weapon is full!"), true, false, FColor::Orange, 5.f);
			return;
		}

		if(CurrentWeaponSettings.CurrentMagazineAmmo < CurrentWeaponSettings.MagazineCapacity)
		{
			PlayerState = EPlayerStates::Reloading;
			PlaySoundAtOwner(CurrentWeaponSettings.ReloadInsertSound);
			if (CurrentWeaponSettings.ReloadInsertAnimation != nullptr)
			{
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(CurrentWeaponSettings.ReloadInsertAnimation, 1.f);
					float currentAnimLength = AnimInstance->GetCurrentActiveMontage()->GetPlayLength();

					GetWorld()->GetTimerManager().SetTimer(
						PumpOrBoltReloadTimerHandle,
						this,
						&UCombatComponent::UnlockBoltOrPumpWeaponReload,
						currentAnimLength,
						false);
					bCanReload = false;
				}
			}
			PlayWeaponAnimation(CurrentWeaponSettings.WeaponReloadInsertAnimation);

			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Continue reload bolt or pump weapon"), true, false, FColor::Orange, 2.f);
		}
	}
}

void UCombatComponent::UnlockReload()
{
	// Notify HUD, play reload animation/sound, etc.
	// 
	// updating weapon bar in blueprints
	PlayerOwner->UpdateWeaponBarCurrentAmmo();
	PlayerOwner->UpdateWeaponBarMaxAmmo();

	bCanReload = true;
	PlayerState = EPlayerStates::Unoccupied;
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Reload Unlocked!"), true, false, FColor::Orange, 3.f);
}

bool UCombatComponent::CanReload()
{
	if (bCanReload &&
		PlayerState == EPlayerStates::Unoccupied)
	{
		return true;
	}
	return false;
}

void UCombatComponent::UnlockBoltOrPumpWeaponReload()
{
	if (FAmmoData* AmmoData = AmmoMap.Find(CurrentWeaponSettings.AmmoType))
	{
		// Reload one bullet
		CurrentWeaponSettings.CurrentMagazineAmmo++;
		AmmoData->CurrentAmmo--;
		UnlockReload();
	}
	HandlePumpOrBoltWeaponReload(CurrentWeaponSettings.AmmoType);
}

void UCombatComponent::PlayAnimationWithDelay(UAnimMontage* InMontage)
{
	if (InMontage != nullptr)
	{
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(InMontage, 1.f);
			float currentAnimLength = AnimInstance->GetCurrentActiveMontage()->GetPlayLength();

			GetWorld()->GetTimerManager().SetTimer(
				ReloadTimerHandle,
				this,
				&UCombatComponent::HandleEndReload,
				currentAnimLength,
				false);
			bCanReload = false;
		}
	}
}

void UCombatComponent::HandleEndReload()
{
	if (CurrentWeaponSettings.IsPumpOrBoltAction)
	{
		HandlePumpOrBoltWeaponReload(CurrentWeaponSettings.AmmoType);
		UnlockReload();
	}
	else
	{
		HandleReload(CurrentWeaponSettings.AmmoType);
		UnlockReload();
	}
}

void UCombatComponent::AttachCurrentWeaponToSocket(FName SocketName)
{
	if (PlayerOwner && CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(
			PlayerOwner->GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform,
			SocketName);
	}
}

bool UCombatComponent::IsPlayerUnnocupied()
{
	return PlayerState == EPlayerStates::Unoccupied;
}

void UCombatComponent::OnStartMantling()
{
	if(PlayerOwner) PlayerOwner->StopSprinting();

	StopADS();
	StopFiring();

	PlayerState = EPlayerStates::Mantling;
	AttachCurrentWeaponToSocket(FName("SOCKET_Back_Weapon"));
}

void UCombatComponent::BroadcastQuickUnholster()
{
	StartFastUnholsterDelegate.Broadcast();
}

void UCombatComponent::PlayWeaponAnimation(UAnimMontage* InMontage)
{
	if (InMontage != nullptr)
	{
		if (CurrentWeaponAnimInstance != nullptr)
		{
			CurrentWeaponAnimInstance->Montage_Play(InMontage, 1.f);
		}
	}
}

void UCombatComponent::PlaySoundAtOwner(USoundBase* InSound)
{
	if (InSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, InSound, PlayerOwner->GetActorLocation());
	}
}



