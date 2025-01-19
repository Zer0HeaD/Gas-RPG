// Copyright Arthur "Zer0HeaD" Zakirov


#include "RPG_AssetManager.h"
#include "RPG_GameplayTags.h"

URPG_AssetManager& URPG_AssetManager::Get()
{
	check(GEngine);


	URPG_AssetManager* RPG_AssetManager = Cast<URPG_AssetManager>(GEngine->AssetManager);
	return *RPG_AssetManager;
}

void URPG_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FRPG_GameplayTags::InitializeNativeGameplayTags();
}
