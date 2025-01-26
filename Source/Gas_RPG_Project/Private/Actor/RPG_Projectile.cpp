// Copyright Arthur "Zer0HeaD" Zakirov


#include "Actor/RPG_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Gas_RPG_Project/Gas_RPG_Project.h"

// Sets default values
ARPG_Projectile::ARPG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->InitialSpeed = 550;
	ProjectileMovement->MaxSpeed = 550;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ARPG_Projectile::BeginPlay()
{
	AActor::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->IgnoreActorWhenMoving(GetInstigator(), true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARPG_Projectile::OnSphereOverlap);


	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(
		LoopingSound, 
		GetRootComponent(),
		FName(),
		FVector::ZeroVector,
		EAttachLocation::KeepRelativeOffset,
		true);
}

void ARPG_Projectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		//LoopingSoundComponent->Stop();
	}
	AActor::Destroyed();
}

void ARPG_Projectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	
	//if(LoopingSoundComponent && LoopingSoundComponent->IsPlaying()) LoopingSoundComponent->Stop();

	if (HasAuthority())
	{
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

