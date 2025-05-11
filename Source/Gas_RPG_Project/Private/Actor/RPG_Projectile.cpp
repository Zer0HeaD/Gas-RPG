// Copyright Arthur "Zer0HeaD" Zakirov


#include "Actor/RPG_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Gas_RPG_Project/Gas_RPG_Project.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RPG_AbilitySystemLibrary.h"

#include "Perception/AISense_Damage.h"
#include "Perception/AIPerceptionSystem.h"

#include "Character/RPG_Gas_Character.h"
#include "Character/RPG_Player_Character.h"


// Sets default values
ARPG_Projectile::ARPG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	ProjectileMovement->InitialSpeed = 1000;
	ProjectileMovement->MaxSpeed = 2500;
	ProjectileMovement->ProjectileGravityScale = 0.15f;

	ProjectileMovement->bInterpMovement = true;
	ProjectileMovement->bInterpRotation = true;
	ProjectileMovement->bThrottleInterpolation = true;

	// Setup Homing
	ProjectileMovement->bIsHomingProjectile = true;
	/*
	*	15'000 - Set as "Normal". Need to at least move to evade projectile. Dash not necessary.
	*	17'000 - Set as "Hard" value. Pretty hard to evade, need a dash
	*	20'000 - Set as "Ultra Hard". Very hard to evade, but still possible
	*	25'000 - Tests showed that nearly imposible to evade. NIGHTMARE!
	*/
	ProjectileMovement->HomingAccelerationMagnitude = 15000.f;
}

// Called when the game starts or when spawned
void ARPG_Projectile::BeginPlay()
{
	AActor::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->IgnoreActorWhenMoving(GetInstigator(), true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARPG_Projectile::OnSphereOverlap);

	// Store Player ref for real time Tracking Homing target
	PlayerTarget = Cast<ARPG_Player_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(
		LoopingSound, 
		GetRootComponent(),
		FName(),
		FVector::ZeroVector,
		EAttachLocation::KeepRelativeOffset,
		true);

	GetWorld()->GetTimerManager().SetTimer(
		ResetHomingTimer,
		this,
		&ARPG_Projectile::ResetHomingProjectile,
		HomingResetTime,
		false);
}

void ARPG_Projectile::ResetHomingProjectile()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->bIsHomingProjectile = false;
		ProjectileMovement->HomingTargetComponent = nullptr;

		FVector LastVelocity = ProjectileMovement->Velocity.GetSafeNormal();
		ProjectileMovement->Velocity = LastVelocity * ProjectileMovement->MaxSpeed;
	}
	SetActorTickEnabled(false);
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

void ARPG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// NOT MULTIPLAYER READY! We find player on a map and set it as a homing target for homing projectile
	if (PlayerTarget && ProjectileMovement)
	{
		//if (TargetComponent && ProjectileMovement->HomingTargetComponent != TargetComponent)
		{
			ProjectileMovement->HomingTargetComponent = PlayerTarget->GetTargetComponent();
		}
	}
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
		SetActorTickEnabled(false);
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
			if (OtherActor != UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) && 
				SweepResult.BoneName == "head")
			{
				UKismetSystemLibrary::PrintString(GetWorld(), TEXT("HEADSHOT!"), true, false, FColor::Red, 5.f);
				DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Physical")), 9999.f);
			}
			Target_ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

			// Temp: Set Damage Amount to 1.f . No need for that at 04.16.2025.
			UAISense_Damage::ReportDamageEvent(
				GetWorld(),
				OtherActor,
				GetInstigator(),
				1.f,
				SweepResult.ImpactPoint,
				OtherActor->GetActorLocation());
		}
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

