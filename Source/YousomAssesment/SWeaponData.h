#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "SWeaponData.generated.h"


USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* MuzzleFlash;

	FWeaponData()
		: Mesh(nullptr)
		, Damage(3.0f)
		, FireRate(0.1f)
		, Range(1000.0f)
		, MuzzleFlash(nullptr)
	{
	}
};