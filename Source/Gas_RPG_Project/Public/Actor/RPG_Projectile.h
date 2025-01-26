// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(VisibleAnywhere) TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION() void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
private:

	UPROPERTY(VisibleAnywhere) TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere) TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY(EditAnywhere) TObjectPtr<USoundBase> ImpactSound;

public:	


};
