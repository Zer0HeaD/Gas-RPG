// Copyright Arthur "Zer0HeaD" Zakirov


#include "Actor/RPG_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Gas_RPG_Project/Gas_RPG_Project.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RPG_AbilitySystemLibrary.h"

#include "Character/RPG_Gas_Character.h"


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
	//if (!bHit && !HasAuthority())
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	//	//LoopingSoundComponent->Stop();
	//}
	AActor::Destroyed();
}

void ARPG_Projectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator()) return;

	// Prevent players and enemies damage itself. For melee it's handled by combo graph
	if (!URPG_AbilitySystemLibrary::IsNotFriend(
		DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(),
		OtherActor))
	{
		return;
	}

	if (ARPG_Gas_Character* HitCharacter = Cast<ARPG_Gas_Character>(OtherActor))
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitCharacter->HitSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitCharacter->HitEffect, GetActorLocation());
	}
	else
	{
		if (BulletHoleDecal)
		{
			UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(),
				BulletHoleDecal, 
				FVector(DecalSize),
				SweepResult.Location,
				SweepResult.ImpactNormal.Rotation(), 
				LifetimeSpanBulletHoleDecal);
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("WARNING: NO BULLET HOLE TO SPAWN!"), true, false, FColor::Red, 5.f);
		}
	}
	
	//if(LoopingSoundComponent && LoopingSoundComponent->IsPlaying()) LoopingSoundComponent->Stop();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* Target_ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			Target_ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		

		Destroy();
	}
	else
	{
		bHit = true;
	}
}

