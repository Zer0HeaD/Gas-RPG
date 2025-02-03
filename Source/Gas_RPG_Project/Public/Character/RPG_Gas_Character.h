// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "RPG_Gas_Character.generated.h"



class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

UCLASS(Abstract)
class GAS_RPG_PROJECT_API ARPG_Gas_Character : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPG_Gas_Character();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	
	virtual void Die() override;
	UFUNCTION(NetMulticast, Reliable) virtual void MulticastHandleDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY() TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY() TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") TObjectPtr<UStaticMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat") FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;
	virtual FRotator GetOwnerRotation() override;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	virtual void InitializeDefaultAttributes() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes") 
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void AddCharacterAbilities();

	/* Dissolve Effects */
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent) void StartDissolveTimeline(/*UMaterialInstanceDynamic* DynamicMaterialInstance*/);
	/// CURRENTLY NOT USING DURING USING MATERIAL FUNCTION
	//UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:

	UPROPERTY(EditAnywhere, Category = "Attributes") TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat") TObjectPtr<UAnimMontage> HitReactMontage;
};
