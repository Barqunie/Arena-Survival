#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"   // ADD
#include "ArenaCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAbilitySystemComponent;
class UGameplayAbility;
class AActor;

UENUM(BlueprintType)
enum class ECombatStyle : uint8
{
    Melee  UMETA(DisplayName = "Melee"),
    Ranged UMETA(DisplayName = "Ranged")
};

UCLASS()
class ARENASURVIVAL_API AArenaCharacterBase
    : public ACharacter
    , public IAbilitySystemInterface   // ADD
{
    GENERATED_BODY()

public:
    AArenaCharacterBase();

    // IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }

    // ACharacter
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    // Combat
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    ECombatStyle CombatStyle = ECombatStyle::Melee;

    // Ranged
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Ranged")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Ranged")
    FName MuzzleSocketName = TEXT("Muzzle");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Ranged")
    float ProjectileSpawnForwardOffset = 30.f;

    // GAS
    UPROPERTY(EditAnywhere, Category = "GAS")
    TSubclassOf<UGameplayAbility> MeleeAutoAbilityClass;

    // Anim hooks
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Anim_FireProjectile();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Anim_BeginMeleeWindow();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Anim_EndMeleeWindow();



protected:
    void Move(const FInputActionValue& Value);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Melee")
    bool bMeleeWindowActive = false;

private:
    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // Input
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    // ASC instance
    UPROPERTY(VisibleAnywhere, Category = "GAS")
    UAbilitySystemComponent* ASC = nullptr;   // keep pointer; create in ctor
};
