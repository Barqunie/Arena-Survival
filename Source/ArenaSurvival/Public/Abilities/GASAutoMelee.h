#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASAutoMelee.generated.h"

class UCharacterStatsComponent;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitDelay;

UCLASS()
class ARENASURVIVAL_API UGA_AutoMelee : public UGameplayAbility
{
    GENERATED_BODY()
public:
    UGA_AutoMelee();

    // BP’den dolduracaksýn (senin GA_BP bu sýnýfýn child’ý)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
    UAnimMontage* MeleeMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0.1"))
    float PlayRate = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0.0"))
    float ExtraDelayAfter = 0.f;

    // UGameplayAbility
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintPure, Category = "Melee")
    float GetAttackIntervalFromStats() const;

private:
    void PlayOneCycle();

    UFUNCTION() void OnMontageCompleted();
    UFUNCTION() void OnMontageBlendOut();
    UFUNCTION() void OnMontageInterrupted();
    UFUNCTION() void OnMontageCancelled();

    void ScheduleNextCycle();

    // task pointer’larý scope bitmeden yaþasýn
    UPROPERTY() UAbilityTask_PlayMontageAndWait* PlayTask = nullptr;
    UPROPERTY() UAbilityTask_WaitDelay* WaitTask = nullptr;

    bool bEnding = false;
};
