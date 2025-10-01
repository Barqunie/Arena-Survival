#include "Character/ArenaMeleeCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Components/Character/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"

AArenaMeleeCharacter::AArenaMeleeCharacter()
{
    // nothing extra yet
}

void AArenaMeleeCharacter::GiveStartupAbilities()
{
    if (!ASC) return;
    if (!HasAuthority()) return;
    if (!MeleeAutoAbilityClass) return;

    FGameplayAbilitySpec Spec(MeleeAutoAbilityClass, 1, 0, this);
    const FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);
    ASC->TryActivateAbility(Handle);
}

void AArenaMeleeCharacter::Anim_BeginMeleeWindow()
{
    bMeleeWindowActive = true;
    MeleeHitThisSwing.Empty();
}

void AArenaMeleeCharacter::Anim_EndMeleeWindow()
{
    bMeleeWindowActive = false;
}

void AArenaMeleeCharacter::Anim_MeleeHitTrace()
{
    if (!bMeleeWindowActive) return;

    // Start at socket or near character
    FVector Start = GetActorLocation() + FVector(0.f, 0.f, 60.f);
    if (USkeletalMeshComponent* Skel = GetMesh())
    {
        if (MeleeSocket != NAME_None && Skel->DoesSocketExist(MeleeSocket))
        {
            Start = Skel->GetSocketLocation(MeleeSocket);
        }
    }
    const FVector End = Start + GetActorForwardVector() * MeleeRange;

    // Sweep
    TArray<FHitResult> Hits;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(MeleeTrace), false, this);
    const bool bHit = GetWorld()->SweepMultiByChannel(
        Hits, Start, End, FQuat::Identity, ECC_Pawn,
        FCollisionShape::MakeSphere(MeleeRadius), Params);

    if (!bHit) return;

    // Damage from Stats (defaults to AttackPower)
    float Damage = 10.f;
    if (const UCharacterStatsComponent* S = FindComponentByClass<UCharacterStatsComponent>())
    {
        Damage = S->AttackPower;
    }

    for (const FHitResult& HR : Hits)
    {
        AActor* Other = HR.GetActor();
        if (!Other || Other == this) continue;
        if (MeleeHitThisSwing.Contains(Other)) continue;

        MeleeHitThisSwing.Add(Other);
        UGameplayStatics::ApplyDamage(Other, Damage, GetController(), this, nullptr);
    }
}
void AArenaMeleeCharacter::GiveStartupAbilities()
{
    if (!ASC || !MeleeAutoAbilityClass) return;

    // 1) Server: ability’i ver
    if (HasAuthority())
    {
        ASC->GiveAbility(FGameplayAbilitySpec(MeleeAutoAbilityClass, 1, 0, this));
        UE_LOG(LogTemp, Log, TEXT("[Melee] Ability GIVEN on server: %s"), *MeleeAutoAbilityClass->GetName());
    }

    // 2) Her iki tarafta da aktive etmeyi dene (client + server)
    const bool bTried = ASC->TryActivateAbilityByClass(MeleeAutoAbilityClass);
    UE_LOG(LogTemp, Log, TEXT("[Melee] TryActivateAbilityByClass: %s"), bTried ? TEXT("TRUE") : TEXT("FALSE"));

    // 3) Ekrana da yaz (PIE’de hýzlý teþhis)
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Melee ability: Give+TryActivate"));
    }
}