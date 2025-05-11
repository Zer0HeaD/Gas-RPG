// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Character/RPG_Gas_Character.h"
#include "UI/WidgetController/RPG_OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "RPG_Enemy_Character.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ARPGAIController;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_Enemy_Character : public ARPG_Gas_Character
{
	GENERATED_BODY()
public:
	ARPG_Enemy_Character(const FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController);

	//virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

protected:
	virtual void BeginPlay() override;
	void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	void BindDelegates();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat") int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat") float KnockbackImpulseForce = 500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UWidgetComponent> InfoWidget;

	UPROPERTY(EditAnywhere, Category = "Combat") TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY() TObjectPtr<ARPGAIController> RPGAIController;

public:

#pragma region Combat Interface
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	UFUNCTION(BlueprintImplementableEvent) void OnDeathEvent();
#pragma endregion

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void HeavyHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat") bool bHitReacting;
	UPROPERTY(BlueprintReadOnly, Category = "Combat") float HitWalkSpeed = 250.f;
	UPROPERTY(BlueprintReadOnly, Category = "Combat") float BaseWalkSpeed = 450.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat") TObjectPtr<AActor> CombatTarget;


	UPROPERTY(BlueprintAssignable) FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable) FOnAttributeChangedSignature OnMaxHealthChanged;

};
