#include "GameMode/ArenaGameModeBase.h"
#include "Character/ArenaCharacterBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AArenaGameModeBase::AArenaGameModeBase()
{
	// Spawn this pawn for PlayerController 0 at a PlayerStart in the level
	DefaultPawnClass = AArenaCharacterBase::StaticClass();
}

void AArenaGameModeBase::Respawn(AController* Controller)
{
	if (!Controller) return;

	// Find a PlayerStart and respawn there
	AActor* StartSpot = FindPlayerStart(Controller);
	if (StartSpot)
	{
		const FTransform StartTM = StartSpot->GetActorTransform();
		RestartPlayerAtTransform(Controller, StartTM);
	}
	else
	{
		RestartPlayer(Controller);
	}
}
