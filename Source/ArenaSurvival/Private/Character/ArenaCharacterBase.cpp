#include "Character/ArenaCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"                 // ADD
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
// #include "NativeGameplayTags.h"              // optional
#include "Abilities/GameplayAbility.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Character/CharacterStatsComponent.h"

AArenaCharacterBase::AArenaCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;      // no tick panel

    // movement
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

    // camera
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 1200.f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritRoll = false;
    CameraBoom->SetUsingAbsoluteRotation(true);
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // ASC
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));  // ADD
    ASC->SetIsReplicated(true);
    ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AArenaCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    // input mapping
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
            {
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }

    // enforce fixed camera
    if (CameraBoom)
    {
        CameraBoom->bUsePawnControlRotation = false;
        CameraBoom->bInheritPitch = false;
        CameraBoom->bInheritYaw = false;
        CameraBoom->bInheritRoll = false;
        CameraBoom->SetUsingAbsoluteRotation(true);
        CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    }
    if (FollowCamera) FollowCamera->bUsePawnControlRotation = false;

    // stats speed
    if (UCharacterStatsComponent* Stats = FindComponentByClass<UCharacterStatsComponent>())
    {
        GetCharacterMovement()->MaxWalkSpeed = FMath::Max(150.f, Stats->MoveSpeed);
    }

    // ASC init
    if (ASC) ASC->InitAbilityActorInfo(this, this);

    // give and start auto-melee on server
    if (ASC && HasAuthority() && CombatStyle == ECombatStyle::Melee && MeleeAutoAbilityClass)
    {
        FGameplayAbilitySpec Spec(MeleeAutoAbilityClass, 1, 0, this);
        FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);
        ASC->TryActivateAbility(Handle);
    }
}

void AArenaCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AArenaCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
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

void AArenaCharacterBase::Anim_FireProjectile()
{
    if (CombatStyle != ECombatStyle::Ranged) return;
    if (!ProjectileClass) return;

    FVector  SpawnLoc = GetActorLocation() + GetActorForwardVector() * ProjectileSpawnForwardOffset + FVector(0.f, 0.f, 60.f);
    FRotator SpawnRot = GetActorRotation();

    if (USkeletalMeshComponent* SkelMesh = GetMesh())
    {
        if (MuzzleSocketName != NAME_None && SkelMesh->DoesSocketExist(MuzzleSocketName))
        {
            SpawnLoc = SkelMesh->GetSocketLocation(MuzzleSocketName);
            SpawnRot = SkelMesh->GetSocketRotation(MuzzleSocketName);
        }
    }

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = this;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLoc, SpawnRot, Params);
}

void AArenaCharacterBase::Anim_BeginMeleeWindow()
{
    bMeleeWindowActive = true;
}

void AArenaCharacterBase::Anim_EndMeleeWindow()
{
    bMeleeWindowActive = false;
}
