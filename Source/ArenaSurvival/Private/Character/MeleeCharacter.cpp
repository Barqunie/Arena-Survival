#include "Character/MeleeCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Components/Character/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"

AArenaMeleeCharacter::AArenaMeleeCharacter()
{
    // nothing extra yet
}

void AArenaMeleeCharacter::GiveStartupAbilities()
{
    if (!ASC || !MeleeAutoAbilityClass) return;


    if (HasAuthority())
    {
        if (!ASC->FindAbilitySpecFromClass(MeleeAutoAbilityClass))
        {
            ASC->GiveAbility(FGameplayAbilitySpec(MeleeAutoAbilityClass, 1, 0, this));
            UE_LOG(LogTemp, Log, TEXT("[Melee] Ability GIVEN on server: %s"),
                *MeleeAutoAbilityClass->GetName());
        }
    }


    const bool bOk = ASC->TryActivateAbilityByClass(MeleeAutoAbilityClass);
    UE_LOG(LogTemp, Log, TEXT("[Melee] TryActivateAbilityByClass: %s"), bOk ? TEXT("TRUE") : TEXT("FALSE"));

#if !UE_BUILD_SHIPPING
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 2.f, bOk ? FColor::Green : FColor::Red,
            TEXT("Melee ability: Give+TryActivate"));
#endif
}

void AArenaMeleeCharacter::Anim_BeginMeleeWindow()
{
    bMeleeWindowActive = true;
    MeleeHitThisSwing.Empty();
}

void AArenaMeleeCharacter::Anim_EndMeleeWindow()
{
    bMeleeWindowActive = false;
}



