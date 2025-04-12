// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h" 
#include "RecoilAnimationComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Actor/RPG_Projectile.h"
#include "ScalableFloat.h"
#include "CombatComponent.generated.h"

class AWeaponBase;
class ARPG_Player_Character;
class USoundBase;
class UAnimMontage;
class UCurveFloat;
class UParticleSystem;
class UMaterialInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FShootProjectileDelegate, TSubclassOf<ARPG_Projectile>, ProjectileClass, FVector, MuzzleLocation, FRotator, LaunchDirection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartFastUnholsterDelegate);

UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	Unoccupied				UMETA(DisplayName = "Unoccupied"),
	Shooting				UMETA(DisplayName = "Shooting"),
	Reloading				UMETA(DisplayName = "Reloading"),
	SwitchingWeapon			UMETA(DisplayName = "SwitchingWeapon"),
	MeleeAtacking			UMETA(DisplayName = "MeleeAtacking"),
	Climbing				UMETA(DisplayName = "Climbing"),
	Mantling				UMETA(DisplayName = "Mantling"),
	GettingReady			UMETA(DisplayName = "GettingReady")
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Ammo_AR			UMETA(DisplayName = "Assault Rifle Ammo"),
	Ammo_SMG		UMETA(DisplayName = "Submachine Gun Ammo"),
	Ammo_Pistol		UMETA(DisplayName = "Pistol Ammo"),
	Ammo_Shotgun	UMETA(DisplayName = "Shotgun Ammo"),
	// Add more ammo types as needed
};

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

public:
	// Current ammo count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Equipment")
	int32 CurrentAmmo;

	// Maximum ammo count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Equipment")
	int32 MaxAmmo;

	/*FAmmoData()
		: CurrentAmmo(0), MaxAmmo(0) {}

	FAmmoData(int32 InCurrentAmmo, int32 InMaxAmmo)
		: CurrentAmmo(InCurrentAmmo), MaxAmmo(InMaxAmmo) {}*/
};

// DEFAULT SETUP OF RECOIL SYSTEM
//float RecoilStat = 85.f;
//float BaseRecoilPitchForce = 5.f;
//float BaseRecoilYawForce = 8.f;
//float BloomStep = 0.5f;
//float ADSBloomModifier = 0.2f;
//float BloomRecoveryInterpSpeed = 20.f;
//float MaxBloom = 10.f;
//float MaxADSHeat = 10.f;
//float ADSheatModifierMax = 0.6f;
USTRUCT(BlueprintType)
struct FWeaponSettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") TMap<FGameplayTag, FScalableFloat> DamageTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") int32 MagazineCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") int32 CurrentMagazineAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") bool IsPumpOrBoltAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PumpOrBolt", meta = (EditCondition = "IsPumpOrBoltAction", EditConditionHides))
	UAnimMontage* ReloadInsertAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PumpOrBolt", meta = (EditCondition = "IsPumpOrBoltAction", EditConditionHides))
	UAnimMontage* ReloadEndAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PumpOrBolt", meta = (EditCondition = "IsPumpOrBoltAction", EditConditionHides))
	USoundBase* ReloadInsertSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PumpOrBolt", meta = (EditCondition = "IsPumpOrBoltAction", EditConditionHides))
	USoundBase* ReloadEndSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PumpOrBolt", meta = (EditCondition = "IsPumpOrBoltAction", EditConditionHides))
	UAnimMontage* WeaponReloadInsertAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PumpOrBolt", meta = (EditCondition = "IsPumpOrBoltAction", EditConditionHides))
	UAnimMontage* WeaponReloadEndAnimation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite) float PumpOrBoltReloadTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float FireRateTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float FireRateAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") TEnumAsByte<EFireMode_PRAS> FireMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") int32 Burst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") int32 NumberOfBulletsPerShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float RecoilStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float BaseRecoilPitchForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float BaseRecoilYawForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float BloomStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float ADSBloomModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float BloomRecoveryInterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float MaxBloom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float MaxADSHeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") float ADSheatModifierMax;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") URecoilData* RecoilAnimData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") TSubclassOf<UCameraShakeBase> RecoilCameraShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") TSubclassOf<ARPG_Projectile> BulletProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Config") TSubclassOf<AActor> BulletShellEject;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects") UParticleSystem* WeaponMuzzleParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects") FRotator WeaponMuzzleParticleRotation;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects") UMaterialInterface* BulletHoleDecal;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* UnholsterSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* HolsterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* UnholsterQuickSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* HolsterQuickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* AimInSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* AimOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* FireSuppressorSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* FireEmptySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* ReloadEmptySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* InspectSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds") USoundBase* GrenadeThrowSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") float UnholsterAnimationEarlyUnlockModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* UnholsterAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* HolsterAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* UnholsterQuickAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* HolsterQuickAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* AimInAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* AimOutAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* FireAimAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* EmptyFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* EmptyFireAimAnimation;

	// For Pump And Bold action weapon - we using this animations as Reload Start and not come back to main reload function

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* ReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* ReloadEmptyAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* ReloadAimAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* ReloadAimEmptyAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* MeleeAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* InspectAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Animations") UAnimMontage* GrenadeThrowAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Animations") TSubclassOf<UAnimInstance> WeaponAnimationBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Animations") UAnimMontage* WeaponFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Animations") UAnimMontage* WeaponReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Animations") UAnimMontage* WeaponEmptyReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Animations") UAnimMontage* WeaponUnholsterAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Animations") UAnimMontage* WeaponHolsterAnimation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_RPG_PROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<TSubclassOf<AWeaponBase>> WeaponsArray;
	UPROPERTY(BlueprintAssignable) FShootProjectileDelegate ShootProjectileDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region FiringAWeapon


	UFUNCTION(BlueprintCallable) void InitializeCurrentWeaponVariables();

	UFUNCTION(BlueprintCallable) void StartFiring();
	UFUNCTION(BlueprintCallable) void StopFiring();

	void OnFire();
	bool CanFire();

	void UnlockFire();

	void ShootAProjectile(const UWorld* World, 
		FVector MuzzleSocketLocation, 
		FHitResult result, 
		FActorSpawnParameters ActorSpawnParams, 
		FVector LaunchDirection);

public:
	void StartADS();
	void StopADS();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EPlayerStates PlayerState = EPlayerStates::Unoccupied;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bInADS = false;

	UFUNCTION(BlueprintCallable) void SwapWeapons(int32 NewWeaponIndex);
protected:

	bool bFiringAWeapon = false;
	bool bUnlockFire = true;
	FTimerHandle AutomaticFireHandle;
	FTimerHandle LockingFireHandle;
	FTimerHandle BurstFireHandle;

	int BurstCurrentShots = 0;
	float BurstLockingTimer = 1.f;

	FTimerHandle UnholsterHandle;
	FTimerHandle HolsterHandle;
	void HandleUnholstering();
	void HandleHolstering();
	bool bIsSwitchingWeapons = false;
	int32 SavedNewWeaponIndex = 0;
	int32 CurrentWeaponIndex = 0;

#pragma endregion

#pragma region PRAS

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	URecoilAnimationComponent* PlayerRecoilAnimComponent;

#pragma endregion

#pragma region RecoilSystem

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombatSystem, meta = (AllowPrivateAccess = "true"))
	AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CombatSystem, meta = (AllowPrivateAccess = "true"))
	FWeaponSettings CurrentWeaponSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombatSystem, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RecoilDirectionCurve;

public:

	float InitialRecoilPitchForce;
	float RecoilPitchDamping;
	float RecoilPitchVelocity;

	float InitialRecoilYawForce;
	float RecoilYawDamping;
	float RecoilYawVelocity;

	void StartRecoilRecovery();
	bool bIsRecoilRecoveryActive;
	bool bIsRecoilNeutral = true;
	FRotator RecoilCheckpoint;

	bool bUpdateRecoilPitchCheckpointInNextShot = false;

	bool bIsRecoilYawRecoveryActive;
	bool bUpdateRecoilYawCheckpointInNextShot = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) float CurrentBloom = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float ADSAlpha = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float CurrentADSHeat = 0.f;


protected:

	UPROPERTY() ARPG_Player_Character* PlayerOwner;
	UPROPERTY() APlayerController* PlayerController;
	UPROPERTY() UAnimInstance* AnimInstance;
	UPROPERTY() UAnimInstance* CurrentWeaponAnimInstance;

	UFUNCTION(BlueprintCallable) void StartRecoil();
	UFUNCTION(BlueprintCallable) void EndRecoil();

	bool bIsRecoilActive = false;

#pragma endregion

#pragma region Weapon Ammo
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatSystem, meta = (AllowPrivateAccess = "true"))
	bool bDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CombatSystem, meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, FAmmoData> AmmoMap;

	UFUNCTION(BlueprintCallable) void AddAmmo(EAmmoType AmmoType, int32 AmountToAdd);
	UFUNCTION(BlueprintCallable) bool ConsumeAmmo(EAmmoType AmmoType, int32 AmountConsume); // CURRENTLY NOT IN USE!
	UFUNCTION(BlueprintCallable) void ReloadWeapon(EAmmoType AmmoType);

	void HandleReload(EAmmoType AmmoType);
	void HandlePumpOrBoltWeaponReload(EAmmoType AmmoType);
	void UnlockReload();
	bool CanReload();
	void UnlockBoltOrPumpWeaponReload();
	FTimerHandle ReloadTimerHandle;
	FTimerHandle PumpOrBoltReloadTimerHandle;
	bool bCanReload = true;

private:
	// HELPER FUNCTIONS

	void PlayAnimationWithDelay(UAnimMontage* InMontage);
	void PlayWeaponAnimation(UAnimMontage* InMontage);
	void PlaySoundAtOwner(USoundBase* InSound);
	void HandleEndReload();
	bool IsPlayerUnnocupied();

#pragma endregion

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE FWeaponSettings GetCurrentWeaponSettings() { return CurrentWeaponSettings; }

	UFUNCTION(BlueprintCallable) void UnholsterWeapon();
	UFUNCTION(BlueprintCallable) void QuickUnholsterWeapon(bool bVaultMantle = false);
	UFUNCTION(BlueprintCallable) void HolsterWeapon();
	UFUNCTION(BlueprintCallable) void AttachCurrentWeaponToSocket(FName SocketName);
	UFUNCTION(BlueprintCallable) void OnStartMantling();
	UFUNCTION(BlueprintCallable) void SetPlayerState(EPlayerStates State) { PlayerState = State; }

	UFUNCTION(BlueprintCallable) void BroadcastQuickUnholster();

	UPROPERTY(BlueprintAssignable) FStartFastUnholsterDelegate StartFastUnholsterDelegate;
};
