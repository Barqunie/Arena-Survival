// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASAutoMelee.generated.h"

/**
 * 
 */
class UAnimMontage;

UCLASS()
class ARENASURVIVAL_API UGA_AutoMelee : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_AutoMelee();

	// set in editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
	UAnimMontage* MeleeMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0.1"))
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (ClampMin = "0.0"))
	float ExtraDelayAfter = 0.0f;

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
		bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION() void OnMontageDone();
	UFUNCTION() void OnDelayFinished();

	float GetAttackIntervalSeconds(const FGameplayAbilityActorInfo* Info) const;

private:
	bool bRunning = false;
};