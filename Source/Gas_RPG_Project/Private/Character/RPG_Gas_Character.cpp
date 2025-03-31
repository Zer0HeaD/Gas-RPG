// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Gas_Character.h"
#include "InputActionValue.h"
#include "AbilitySystem/RPG_AbilitySystemComponent.h"
#include "Player/RPG_PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include <Gas_RPG_Project/Gas_RPG_Project.h>

// Sets default values
ARPG_Gas_Character::ARPG_Gas_Character(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)  // Make sure to call the base class constructor
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("Greystone_Sword_Socket"));

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//GetMesh()->VisibilityBasedAnimTickOption(EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones);
}

UAbilitySystemComponent* ARPG_Gas_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ARPG_Gas_Character::GetHitReactMontage_Implementation()
{
	check(HitReactMontage);
	return HitReactMontage;
}

void ARPG_Gas_Character::Die()
{
	//TODO: DROP WEAPON ON DEATH
	MulticastHandleDeath();
}

UNiagaraSystem* ARPG_Gas_Character::GetHitEffect_Implementation()
{
	return HitEffect;
}

USoundBase* ARPG_Gas_Character::GetHitSound_Implementation()
{
	return HitSound;
}

UAnimMontage* ARPG_Gas_Character::GetDeathMontage_Implementation()
{
	return DeathMontage;
}

int32 ARPG_Gas_Character::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ARPG_Gas_Character::SetMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
	if (MinionCount < 0)
	{
		MinionCount = 0;
	}
}

UCombatComponent* ARPG_Gas_Character::GetCombatComponent_Implementation()
{
	return nullptr;
}

void ARPG_Gas_Character::MulticastHandleDeath_Implementation()
{
	//TODO: Simulate physics of weapon
	//TODO: Return used Token in blueprints (enemy can dead while attacking)
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Dissolve();
}

// Called when the game starts or when spawned
void ARPG_Gas_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ARPG_Gas_Character::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);
}

void ARPG_Gas_Character::InitAbilityActorInfo()
{
}

FVector ARPG_Gas_Character::GetCombatSocketLocation()
{
	check(Weapon);
	return 	GetMesh()->GetSocketLocation(WeaponTipSocketName);
}

FRotator ARPG_Gas_Character::GetOwnerRotation()
{
	return bUseControllerRotationYaw ? GetControlRotation() : GetActorRotation();
}

void ARPG_Gas_Character::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle =
	GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ARPG_Gas_Character::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

// EXECUTE ONLY ON A SERVER
void ARPG_Gas_Character::AddCharacterAbilities()
{
	URPG_AbilitySystemComponent* ASC = CastChecked<URPG_AbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
}

void ARPG_Gas_Character::Dissolve()
{
	/*if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		for (int index = 0; index < GetMesh()->GetNumMaterials(); ++index)
		{
			GetMesh()->SetMaterial(index, DynamicMatInst);
			StartDissolveTimeline(DynamicMatInst);
		}
	}*/
	// AT THIS POINT WE USE MATERIAL FUNCTION WHICH ALREADY IN CHARACTER MATERIAL
	//for (int index = 0; index < GetMesh()->GetNumMaterials(); ++index)
	{
		StartDissolveTimeline();
	}

	//TODO: Dissolve weapon material. Some weapons skeletal meshses, some static, still don't know which gonna in final project
	/*if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
	}*/
}


