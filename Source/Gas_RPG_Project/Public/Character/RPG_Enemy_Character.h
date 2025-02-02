// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Character/RPG_Gas_Character.h"
#include "UI/WidgetController/RPG_OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "RPG_Enemy_Character.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_Enemy_Character : public ARPG_Gas_Character
{
	GENERATED_BODY()
public:
	ARPG_Enemy_Character();

protected:
	virtual void BeginPlay() override;
	void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	void BindDelegates();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character class defaults") int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character class defaults") 
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UWidgetComponent> InfoWidget;

public:

#pragma region Combat Interface

	virtual int32 GetPlayerLevel() override;

#pragma endregion

	UPROPERTY(BlueprintAssignable) FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable) FOnAttributeChangedSignature OnMaxHealthChanged;
};
