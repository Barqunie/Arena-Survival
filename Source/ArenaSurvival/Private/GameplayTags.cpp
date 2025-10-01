#include "GameplayTags.h"
#include "NativeGameplayTags.h" 

// TANIMLARI AYNI NAMESPACE ÝÇÝNDE YAP
namespace Tags
{
    UE_DEFINE_GAMEPLAY_TAG(Ability_Auto_Melee, "Ability.Auto.Melee");
    UE_DEFINE_GAMEPLAY_TAG(Ability_Auto_Ranged, "Ability.Auto.Ranged");

    UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Fire, "Event.Attack.Fire");
    UE_DEFINE_GAMEPLAY_TAG(Event_Attack_WindowBegin, "Event.Attack.WindowBegin");
    UE_DEFINE_GAMEPLAY_TAG(Event_Attack_WindowEnd, "Event.Attack.WindowEnd");
}
