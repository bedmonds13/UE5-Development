// Fill out your copyright notice in the Description page of Project Settings.


#include "DropWidget.h"
#include "../DragWidget.h"
#include "../InventorySlotWidget.h"
#include "../Item.h"
#include "../InventoryComponent.h"

bool UDropWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UDragWidget* DragWidget = Cast<UDragWidget>(InOperation);
	if (DragWidget)
	{
		UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(DragWidget->WidgetReference);

		if (SlotWidget )
		{
			SlotWidget->RemoveItem();
		}
	}
	return true;
}