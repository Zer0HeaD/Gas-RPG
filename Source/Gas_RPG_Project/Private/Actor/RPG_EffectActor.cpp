// Copyright Arthur "Zer0HeaD" Zakirov


#include "Actor/RPG_EffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RPG_AttributeSet.h"

// Sets default values
ARPG_EffectActor::ARPG_EffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ARPG_EffectActor::OnOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// TODO: change this to apply a Gameplay Effect. For now, using const cast as a hack!
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const URPG_AttributeSet* RPG_AttributeSet =
			Cast<URPG_AttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(URPG_AttributeSet::StaticClass()));

		URPG_AttributeSet* Mutable_RPG_AttributeSet = const_cast<URPG_AttributeSet*>(RPG_AttributeSet);

		Mutable_RPG_AttributeSet->SetHealth(RPG_AttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void ARPG_EffectActor::EndOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{

}

// Called when the game starts or when spawned
void ARPG_EffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARPG_EffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ARPG_EffectActor::EndOverlap);
}

