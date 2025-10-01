#pragma once
#include "Character/ArenaCharacterBase.h"
#include "CoreMinimal.h"
#include "MeleeCharacter.generated.h"

class UGameplayAbility;

/**
 * Melee-only character: auto-attack ability + melee hit trace + notify hooks
 */
UCLASS()
class ARENASURVIVAL_API AArenaMeleeCharacter : public AArenaCharacterBase
{
    GENERATED_BODY()

public:
    AArenaMeleeCharacter();

protected:
    virtual void GiveStartupAbilities() override;

public:
    // Auto-montage ability class (BP: GA_AutoAttack)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee|Ability")
    TSubclassOf<UGameplayAbility> MeleeAutoAbilityClass;


    // Montage side is handled by the ability. Hit timing via notifies:
    UFUNCTION(BlueprintCallable, Category = "Melee")
    void Anim_BeginMeleeWindow();

    UFUNCTION(BlueprintCallable, Category = "Melee")
    void Anim_EndMeleeWindow();

    UFUNCTION(BlueprintCallable, Category = "Melee")
    void Anim_MeleeHitTrace();

    // Simple melee sweep config
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "50"))
    float MeleeRange = 220.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "10"))
    float MeleeRadius = 45.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
    FName MeleeSocket = TEXT("hand_r");

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee")
    bool bMeleeWindowActive = false;



private:
    // prevent multiple hits on same target within one swing
    TSet<TWeakObjectPtr<AActor>> MeleeHitThisSwing;
};
