#include "Items/WeaponBase.h"
#include "Character/ArenaCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeaponBase::OnEquip(AArenaCharacterBase* NewOwner)
{
	OwnerCharacter = NewOwner;
	if (OwnerCharacter && OwnerCharacter->GetMesh())
	{
		AttachToComponent(
			OwnerCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			AttachSocketName
		);
	}
}
