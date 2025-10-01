#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ArenaCharacterBase.generated.h"

class USpringArmComponent; class UCameraComponent;
class UInputMappingContext; class UInputAction;
struct FInputActionValue;
class UAbilitySystemComponent; class UCharacterStatsComponent;

UCLASS(HideCategories = ("Tick"))
class ARENASURVIVAL_API AArenaCharacterBase
    : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()
public:
    AArenaCharacterBase();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }

    // lifecycle
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PossessedBy(AController* NewController) override;       
    virtual void OnRep_PlayerState() override;                          

protected:
    void Move(const FInputActionValue& Value);

    // türevlerin baþlangýçta vereceði ability’ler
    virtual void GiveStartupAbilities() {}

    void InitASC();                                                      

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
    UAbilitySystemComponent* ASC;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    UCharacterStatsComponent* Stats;

private:
    bool bASCInitialized = false;               
    bool bStartupGiven = false;                 
};
