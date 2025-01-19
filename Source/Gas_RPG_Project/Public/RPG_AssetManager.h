// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RPG_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static URPG_AssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
