// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
class UInventoryWidget;
FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}
void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragWidget* DragDropOperation = NewObject<UDragWidget>();

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	DragDropOperation->WidgetReference = this;
	DragDropOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());;

	DragDropOperation->DefaultDragVisual = this;
	DragDropOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = DragDropOperation;
}

void UInventorySlotWidget::RemoveItem()
{
	InventoryRef->RemoveIndex(index);
	this->ItemName = "";
	this->ItemImage = FSlateBrush::FSlateBrush();
	ImageComponent->SetOpacity(0);
	SlotFilled = false;
}

FReply UInventorySlotWidget::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if (InMouseEvent.GetEffectingButton() == DragKey /*|| PointerEvent.IsTouchEvent()*/)
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();

		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}
bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool DropSuccess = false;
	if (UDragWidget* DragWidget = Cast<UDragWidget>(InOperation))
	{
		if (UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(DragWidget->WidgetReference))
		{
			if (SlotWidget->SlotFilled && this->SlotFilled)
			{
				if (SlotWidget->index == this->index )
				{
					DropSuccess = true;
				}
				else
				{
					InventoryRef->SwapItems(SlotWidget->index, this->index);
					InventoryRef->ResetWidget();
				}
			}
		}
	}
	return DropSuccess;
}

void UInventorySlotWidget::InitializeWidget(UInventoryComponent* Inventory,  int32 Newindex)
{
	InventoryRef = Inventory;
	index = Newindex;
}

void UInventorySlotWidget::FillWidget(FString NewItemName, FSlateBrush NewItemImage)
{
	this->ItemName = NewItemName;
	this->ItemImage = NewItemImage;
	ImageComponent->SetOpacity(1);
	SlotFilled = true;
}