// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDropWidget : public UUserWidget
{
	GENERATED_BODY()

		virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
		
};
