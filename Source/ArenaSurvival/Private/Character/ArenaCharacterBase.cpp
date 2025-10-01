#include "Character/ArenaCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "Components/Character/CharacterStatsComponent.h"

AArenaCharacterBase::AArenaCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;

    bUseControllerRotationYaw = bUseControllerRotationPitch = bUseControllerRotationRoll = false;
    auto* MoveComp = GetCharacterMovement();
    MoveComp->bOrientRotationToMovement = true;
    MoveComp->RotationRate = FRotator(0, 500, 0);
    MoveComp->MaxWalkSpeed = 500; MoveComp->MinAnalogWalkSpeed = 20;
    MoveComp->BrakingDecelerationWalking = 2000; MoveComp->BrakingDecelerationFalling = 1500;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 1200;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritPitch = CameraBoom->bInheritYaw = CameraBoom->bInheritRoll = false;
    CameraBoom->SetUsingAbsoluteRotation(true);
    CameraBoom->SetRelativeRotation(FRotator(-60, 0, 0));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
    ASC->SetIsReplicated(true);
    ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    Stats = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("Stats"));
}

void AArenaCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
        {
            if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
            {
                if (DefaultMappingContext) Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }

    if (Stats) GetCharacterMovement()->MaxWalkSpeed = FMath::Max(150.f, Stats->MoveSpeed);

    InitASC();                                  // 1) BeginPlay’te dene
    if (!bStartupGiven && ASC) { GiveStartupAbilities(); bStartupGiven = true; }
}

void AArenaCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    InitASC();                                  // 2) Possession geldiðinde de dene
    if (!bStartupGiven && ASC) { GiveStartupAbilities(); bStartupGiven = true; }
}

void AArenaCharacterBase::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    InitASC();                                  // 3) client için de güvence
    if (!bStartupGiven && ASC) { GiveStartupAbilities(); bStartupGiven = true; }
}

void AArenaCharacterBase::InitASC()
{
    if (bASCInitialized || !ASC) return;

    // Owner'ý Controller varsa onu yap; yoksa karakterin kendisi
    AActor* OwnerForASC = Cast<AActor>(GetController());
    if (!OwnerForASC) OwnerForASC = this;

    ASC->InitAbilityActorInfo(OwnerForASC, this);
    bASCInitialized = true;
}

void AArenaCharacterBase::Tick(float DeltaSeconds) { Super::Tick(DeltaSeconds); }

void AArenaCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArenaCharacterBase::Move);
            EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &AArenaCharacterBase::Move);
        }
    }
}

void AArenaCharacterBase::Move(const FInputActionValue& Value)
{
    const FVector2D Input = Value.Get<FVector2D>();
    if (Input.IsNearlyZero()) return;
    AddMovementInput(FVector::ForwardVector, Input.Y);
    AddMovementInput(FVector::RightVector, Input.X);
}
