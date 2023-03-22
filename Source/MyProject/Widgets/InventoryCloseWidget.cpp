// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCloseWidget.h"

FReply UInventoryCloseWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(FInventoryCloseWidgetSignature_OnClick.IsBound())
	{
		FInventoryCloseWidgetSignature_OnClick.Execute();
	}
	return FReply::Handled();
}
