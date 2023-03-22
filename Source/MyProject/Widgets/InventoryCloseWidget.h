// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "../InventoryComponent.h"
#include "InventoryCloseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UInventoryCloseWidget : public UUserWidget
{
	GENERATED_BODY()

		DECLARE_DYNAMIC_DELEGATE(FInventoryCloseWidgetSignature); 
public:
		FInventoryCloseWidgetSignature FInventoryCloseWidgetSignature_OnClick;
		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
