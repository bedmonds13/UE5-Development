// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/InputAction.h"
#include "Containers/List.h"
#include "CharacterAttackInputComponent.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		int32 InputDelay;
};

USTRUCT(BlueprintType)
struct FCharacterCombo
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		UInputAction* InputAction;
	
		UPROPERTY(EditAnywhere)
		TArray<FCharacterAttack> CharacterAttack;
				
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCharacterAttackInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttackInputComponent();
		
	
	UPROPERTY(EditDefaultsOnly)
	FCharacterCombo LightAttackCombo;

	UPROPERTY(EditDefaultsOnly)
	FCharacterCombo HeavyAttackCombo;

	UPROPERTY(EditAnywhere)
		bool UseDebugs;
	

	FTimerHandle ComboResetTimer;
	FTimerHandle ComboInputDelay;

private:
	UAnimMontage* CurrentMontage;
	FCharacterCombo* CurrentComboConfig;
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
	bool IsComboInterrupted(FCharacterCombo& Combo);
	void Attack();


	UFUNCTION()
	void ResetCombo_Delay(UAnimMontage* Montage, bool Interrupted);
	
	
private:
	void HandleComboDelay();

	
};
