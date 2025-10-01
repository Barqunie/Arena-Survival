#pragma once
#include "UObject/Interface.h"
#include "AttackSource.generated.h"

// UAttackSource: reflection/blueprint taraf�
UINTERFACE(BlueprintType)
class ARENASURVIVAL_API UAttackSource : public UInterface
{
	GENERATED_BODY()
};

// IAttackSource: C++ taraf� (�a��raca��m�z metotlar)
class ARENASURVIVAL_API IAttackSource
{
	GENERATED_BODY()

public:
	// Anim notifies buraya �a�r� yapacak
	virtual void BeginAttackWindow() {}
	virtual void EndAttackWindow() {}
	virtual void Fire() {}
};
