// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/InputAction.h"
#include "Containers/List.h"
#include "CharacterAttackInputComponent.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAttackConfig
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		UInputAction* InputAction;

		UPROPERTY(EditAnywhere)
		TArray<class UAnimMontage *> AnimMontages;
				
};
UENUM(BlueprintType)
enum AttackType
{
	NONE UMETA(DisplayName = "NONE"),
	Light UMETA(DisplayName = "Light"),
	Heavy UMETA(DisplayName = "Heavy")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCharacterAttackInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttackInputComponent();
		
	
	UPROPERTY(EditDefaultsOnly)
	FCharacterAttackConfig LightAttackCombo;

	UPROPERTY(EditDefaultsOnly)
	FCharacterAttackConfig HeavyAttackCombo;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<AttackType> CurrentAttackType;

	FTimerHandle ComboResetTimer;
	FTimerHandle ComboInputDelay;
	UAnimMontage* CurrentMontage;

private:
	FCharacterAttackConfig* CurrentComboConfig;
	int32 current_index;
	bool InputEnabled;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BindInput(UInputComponent* PlayerInputComponent, APlayerController* PC);
	void LightAttack(const FInputActionValue& Value);
	void HeavyAttack(const FInputActionValue& Value);
	void ResetCombo();

	UFUNCTION()
	void ResetCombo_Delay(UAnimMontage* Montage, bool Interrupted);
	
	
private:
	void HandleComboDelay();

	
};
