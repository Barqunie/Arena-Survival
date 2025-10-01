#include "Abilities/GASAutoMelee.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Character/ArenaCharacterBase.h"
#include "Components/Character/CharacterStatsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_AutoMelee::UGA_AutoMelee()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated; // veya LocalPredicted
}

void UGA_AutoMelee::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, /*rep*/true, /*cancelled*/true);
        return;
    }

    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid() || !MeleeMontage)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    bEnding = false;
#if !UE_BUILD_SHIPPING
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("GA_AutoMelee: Activate"));
#endif

    PlayOneCycle();
}

void UGA_AutoMelee::PlayOneCycle()
{
    if (!MeleeMontage || !CurrentActorInfo) { return; }

    PlayTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this, NAME_None, MeleeMontage, PlayRate, NAME_None,
        /*bStopWhenAbilityEnds*/ true, /*AnimRootMotion*/ 1.0f,
        /*StartTimeSeconds*/ 0.f, /*bAllowInterruptAfterBlendOut*/ true);

    if (!PlayTask) { return; }

    PlayTask->OnCompleted.AddDynamic(this, &UGA_AutoMelee::OnMontageCompleted);
    PlayTask->OnBlendOut.AddDynamic(this, &UGA_AutoMelee::OnMontageBlendOut);
    PlayTask->OnInterrupted.AddDynamic(this, &UGA_AutoMelee::OnMontageInterrupted);
    PlayTask->OnCancelled.AddDynamic(this, &UGA_AutoMelee::OnMontageCancelled);
    PlayTask->ReadyForActivation();
}

void UGA_AutoMelee::OnMontageCompleted() { ScheduleNextCycle(); }
void UGA_AutoMelee::OnMontageBlendOut() { ScheduleNextCycle(); }

void UGA_AutoMelee::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_AutoMelee::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_AutoMelee::ScheduleNextCycle()
{
    if (bEnding) return;

    const float Interval = GetAttackIntervalFromStats() + ExtraDelayAfter;

    WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, FMath::Max(0.01f, Interval));
    WaitTask->OnFinish.AddDynamic(this, &UGA_AutoMelee::PlayOneCycle);
    WaitTask->ReadyForActivation();
}

float UGA_AutoMelee::GetAttackIntervalFromStats() const
{
    if (!CurrentActorInfo) return 0.35f;

    if (const AActor* Avatar = CurrentActorInfo->AvatarActor.Get())
    {
        if (const AArenaCharacterBase* C = Cast<AArenaCharacterBase>(Avatar))
        {
            if (const UCharacterStatsComponent* S = C->FindComponentByClass<UCharacterStatsComponent>())
            {
                return FMath::Max(0.05f, S->AttackSpeed);
            }
        }
    }
    return 0.35f;
}

void UGA_AutoMelee::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    bEnding = true;
u
    // task referanslarýný býrak
    PlayTask = nullptr;
    WaitTask = nullptr;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
