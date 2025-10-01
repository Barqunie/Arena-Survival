#include "Abilities/GASAutoMelee.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Character/ArenaCharacterBase.h"
#include "Components/Character/CharacterStatsComponent.h"

UGA_AutoMelee::UGA_AutoMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_AutoMelee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ActorInfo || !MeleeMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return;
	}

	bRunning = true;

	if (UAbilityTask_PlayMontageAndWait* Task =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, MeleeMontage, PlayRate, NAME_None, false, 1.f, 0.f, false))
	{
		Task->OnCompleted.AddDynamic(this, &UGA_AutoMelee::OnMontageDone);
		Task->OnInterrupted.AddDynamic(this, &UGA_AutoMelee::OnMontageDone);
		Task->OnCancelled.AddDynamic(this, &UGA_AutoMelee::OnMontageDone);
		Task->ReadyForActivation();
	}
}

void UGA_AutoMelee::OnMontageDone()
{
	if (!bRunning) return;

	const FGameplayAbilityActorInfo* Info = GetCurrentActorInfo();
	float Interval = GetAttackIntervalSeconds(Info) + ExtraDelayAfter;
	Interval = FMath::Max(0.05f, Interval);

	if (UAbilityTask_WaitDelay* Delay = UAbilityTask_WaitDelay::WaitDelay(this, Interval))
	{
		Delay->OnFinish.AddDynamic(this, &UGA_AutoMelee::OnDelayFinished);
		Delay->ReadyForActivation();
	}
}

void UGA_AutoMelee::OnDelayFinished()
{
	if (!bRunning || !MeleeMontage) return;

	// replay montage
	if (UAbilityTask_PlayMontageAndWait* Task =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, MeleeMontage, PlayRate, NAME_None, false, 1.f, 0.f, false))
	{
		Task->OnCompleted.AddDynamic(this, &UGA_AutoMelee::OnMontageDone);
		Task->OnInterrupted.AddDynamic(this, &UGA_AutoMelee::OnMontageDone);
		Task->OnCancelled.AddDynamic(this, &UGA_AutoMelee::OnMontageDone);
		Task->ReadyForActivation();
	}
}

float UGA_AutoMelee::GetAttackIntervalSeconds(const FGameplayAbilityActorInfo* Info) const
{
	float Interval = 0.35f;
	if (const AArenaCharacterBase* C = Info ? Cast<AArenaCharacterBase>(Info->AvatarActor.Get()) : nullptr)
	{
		if (const UCharacterStatsComponent* S = C->FindComponentByClass<UCharacterStatsComponent>())
			Interval = FMath::Max(0.05f, S->AttackSpeed);
	}
	return Interval;
}

void UGA_AutoMelee::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	bRunning = false;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
