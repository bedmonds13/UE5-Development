// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboAnimNotify.h"
#include "BaseCharacter.h"
#include "CharacterAttackInputComponent.h"

void UComboAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UCharacterAttackInputComponent* Component = MeshComp->GetOwner()->FindComponentByClass<UCharacterAttackInputComponent>();

	if(Component)
		Component->ResetCombo();
		


}
