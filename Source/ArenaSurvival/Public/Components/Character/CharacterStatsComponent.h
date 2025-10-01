#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"

// Blueprint’ten baðlayacaðýn event’ler:

UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class ARENASURVIVAL_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatsComponent();

	// ---- Base Stats ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "1", UIMin = "1"))
	float MaxHealth = 100.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "1", UIMin = "1"))
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", UIMin = "0"))
	float AttackPower = 20.f;          // vuruþ hasarý katsayýsý

	// 0.35 sn = hýzlý; ileride 1/APS olarak kullanýrsýn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.05", UIMin = "0.05"))
	float AttackSpeed = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", UIMin = "0"))
	float Range = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "150", UIMin = "150"))
	float MoveSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", UIMin = "0", UIMax = "100"))
	float CritChance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "1", UIMin = "100"))
	float CritMultiplier = 1.5f;

protected:
	virtual void BeginPlay() override;
};
