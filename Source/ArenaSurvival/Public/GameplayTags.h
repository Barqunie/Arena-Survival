#pragma once
#include "GameplayTagContainer.h"

// T�m deklarasyonlar ayn� namespace i�inde
namespace Tags
{
    // Abilities
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Auto_Melee);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Auto_Ranged);

    // Events
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Attack_Fire);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Attack_WindowBegin);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Attack_WindowEnd);
}
