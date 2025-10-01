// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/CharacterStatsComponent.h"


UCharacterStatsComponent::UCharacterStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

