// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "RPG_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;


UCLASS()
class GAS_RPG_PROJECT_API ARPG_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPG_Projectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true)) FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
private:
	bool bHit = false;
	AActor* PlayerTarget = nullptr;
	FTimerHandle ResetHomingTimer;
	void ResetHomingProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) TObjectPtr<USphereComponent> Sphere;
	UPROPERTY(EditAnywhere, Category = "Settings") float HomingResetTime = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Settings") float LifeSpan = 15.f;
	UPROPERTY(EditAnywhere, Category = "Settings") TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY(EditAnywhere, Category = "Settings") TObjectPtr<UMaterialInterface> BulletHoleDecal;
	UPROPERTY(EditAnywhere, Category = "Settings") float DecalSize = 15.f;
	UPROPERTY(EditAnywhere, Category = "Settings") float LifetimeSpanBulletHoleDecal = 15.f;
	UPROPERTY(EditAnywhere, Category = "Settings") TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY(EditAnywhere, Category = "Settings") TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY() TObjectPtr<UAudioComponent> LoopingSoundComponent;

public:	


};
