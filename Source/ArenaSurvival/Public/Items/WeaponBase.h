
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AttackSource.h"
#include "WeaponBase.generated.h"

class AArenaCharacterBase;

UCLASS(Abstract)
class ARENASURVIVAL_API AWeaponBase : public AActor, public IAttackSource
{
	GENERATED_BODY()
public:
	AWeaponBase();

	virtual void OnEquip(AArenaCharacterBase* NewOwner);

	// IAttackSource (þimdilik boþ; alt sýnýflar override eder)
	virtual void BeginAttackWindow() override {}
	virtual void EndAttackWindow() override {}
	virtual void Fire() override {}

protected:
	UPROPERTY()
	AArenaCharacterBase* OwnerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName AttachSocketName = TEXT("hand_r");
};
