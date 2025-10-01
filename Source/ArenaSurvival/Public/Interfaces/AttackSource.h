#pragma once
#include "UObject/Interface.h"
#include "AttackSource.generated.h"

// UAttackSource: reflection/blueprint tarafý
UINTERFACE(BlueprintType)
class ARENASURVIVAL_API UAttackSource : public UInterface
{
	GENERATED_BODY()
};

// IAttackSource: C++ tarafý (çaðýracaðýmýz metotlar)
class ARENASURVIVAL_API IAttackSource
{
	GENERATED_BODY()

public:
	// Anim notifies buraya çaðrý yapacak
	virtual void BeginAttackWindow() {}
	virtual void EndAttackWindow() {}
	virtual void Fire() {}
};
