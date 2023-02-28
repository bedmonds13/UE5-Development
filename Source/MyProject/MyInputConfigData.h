#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "MyInputConfigData.generated.h"

UCLASS()
class MYPROJECT_API UMyInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputLook;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputJump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputLightAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputHeavyAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputComboAttack;
};
