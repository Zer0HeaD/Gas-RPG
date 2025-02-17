// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/Abilities/RPG_SummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> URPG_SummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChoosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(
			Hit,
			ChoosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			ChoosenSpawnLocation - FVector(0.f, 0.f, 400.f),
			ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChoosenSpawnLocation = Hit.ImpactPoint;
		}

		SpawnLocations.Add(ChoosenSpawnLocation);

		// DEBUG
		/*DrawDebugSphere(GetWorld(), ChoosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Location,
			Location + Direction * MaxSpawnDistance,
			4.f,
			FLinearColor::Green,
			3.f);
		DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 7.f, 12, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 7.f, 12, FColor::Red, false, 3.f);*/
	}

	return SpawnLocations;
}

TSubclassOf<APawn> URPG_SummonAbility::GetRandomMinionClass()
{
	int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
