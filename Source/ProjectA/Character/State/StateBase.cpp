// Fill out your copyright notice in the Description page of Project Settings.


#include "StateBase.h"
#include "Character/Player/C_Player.h"
#include "Gameplay/PC_Player.h"
#include "Data/ST_Item.h"
#include "Data/InventorySystemComponent.h"
#include "Character/EquipItem.h"

void UStateBase::EnterState(AC_Player* Player)
{
	PlayerReference = Player;
	if (PlayerReference->GetPlayerController() && PlayerReference->GetPlayerController()->GetInventorySystemComponent())
	{
		InputChangeEquip(PlayerReference->GetPlayerController()->GetInventorySystemComponent()->GetEquipItem(0), Cast<USkeletalMeshComponent>(PlayerReference->GetMainWeapon()));
		InputChangeEquip(PlayerReference->GetPlayerController()->GetInventorySystemComponent()->GetEquipItem(1), Cast<USkeletalMeshComponent>(PlayerReference->GetSubWeapon()));
	}
}

void UStateBase::InputMove(FVector2D MoveVector)
{
	const FRotator YawRotation(0, PlayerReference->GetPlayerController()->GetControlRotation().Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PlayerReference->AddMovementInput(ForwardDirection, MoveVector.Y);
	PlayerReference->AddMovementInput(RightDirection, MoveVector.X);
}

void UStateBase::InputLook(FVector2D LookVector)
{
	PlayerReference->AddControllerYawInput(LookVector.X);
	PlayerReference->AddControllerPitchInput(LookVector.Y);
}

void UStateBase::InputJump()
{
	PlayerReference->Jump();
}

void UStateBase::InputChangeEquip(FST_Item* item, USkeletalMeshComponent* EquipComponent)
{
	if (!EquipComponent || !item)
		return;

	EquipComponent->SetSkeletalMesh(item->SkeletalMesh?item->SkeletalMesh : NULL);

	FAttachmentTransformRules attachrules =
		FAttachmentTransformRules::FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget, true);

	//EquipComponent->AttachToComponent(EquipComponent->GetAttachParent(), attachrules, PlayerReference->EState == EStateType::Attack ? item->ASocket : item->BSocket);
	EquipComponent->AttachToComponent(EquipComponent->GetAttachParent(), attachrules, item->ASocket);
	/*EquipComponent->SetRelativeTransform(isEquip ? 
		item->ItemType == EItemType::Tp_TwinDagger ?  
		FTransform(FRotator3d(190.0f, 0, 0), FVector3d(1.4f, 0, 10.0f), FVector3d::One()):
		item->ATransform : item->BTransform);*/
	EquipComponent->SetRelativeTransform(item->ATransform);
}
