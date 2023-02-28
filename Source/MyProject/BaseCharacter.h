// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyInputConfigData.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "MyProjectCharacter.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MYPROJECT_API ABaseCharacter : public AMyProjectCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputAction* InputSequenceAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UMyInputConfigData* InputActions;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		class UAnimMontage* LightAttackAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		class UAnimMontage* HeavyAttackAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		class UAnimMontage* SequenceAttackAnim;

	UPROPERTY(BlueprintReadOnly)
		FTimerHandle AttackTimerHandle;

	

	bool CanAttack;
	//Combo Attack
	int32 comboindex_previous ;
	int32 comboindex_next;
public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CheckCombo(UAnimMontage* NextMontage);
	void ResetCombo();
	void LightAttack(const FInputActionValue& Value);
	void HeavyAttack(const FInputActionValue& Value);
	void ComboAttack(const FInputActionValue& Value);
	void SequenceAttack(const FInputActionValue& Value);
	void SetInputDisableTimer(float TimerRate);
	void EnablePlayerInput();

	UFUNCTION()
	void Event_OnMontageEnded(UAnimMontage* Montage, bool Interrupted);
};
