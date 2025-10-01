#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameModeBase.generated.h"

UCLASS()
class ARENASURVIVAL_API AArenaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaGameModeBase();


	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void Respawn(AController* Controller);
};
