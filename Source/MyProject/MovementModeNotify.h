#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MovementModeNotify.generated.h"

UCLASS()
class MYPROJECT_API UMovementModeNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementMode> MovementMode;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
