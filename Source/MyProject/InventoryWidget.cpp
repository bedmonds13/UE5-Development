// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"

void UInventoryWidget::FillSlots()
{
	if (InventoryRef)
	{
		int index = 0;
		TArray<UWidget*>GridWidgets = GridPanel->GetAllChildren();
		for (UWidget* Widget : GridWidgets)
		{
			UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(Widget);
			if (SlotWidget)
			{
				TArray<TSubclassOf<AItem>> Inventory = InventoryRef->GetInventoryItems();
				if (Inventory.IsValidIndex(index))
				{			
					TSubclassOf<AItem> ItemClass = Inventory[index];
					AItem* ItemRef = Cast<AItem>(ItemClass.GetDefaultObject());	
					SlotWidget->FillWidget(ItemRef->ItemName, ItemRef->ItemImage);
				}
			}
			index++;
		}
	}
}




void UInventoryWidget::NativeConstruct()
{
	
}


void UInventoryWidget::InitializeWidget(UInventoryComponent* InventoryItems)
{
	InventoryRef = InventoryItems;
	CloseButton->OnClicked.AddDynamic(InventoryRef, &UInventoryComponent::ToggleInventory);
	CreateSlots();
	FillSlots();
}

void UInventoryWidget::ClearSlots()
{
	GridPanel->ClearChildren();
}
void UInventoryWidget::Reset()
{
	ClearSlots();
	CreateSlots();
	FillSlots();
}

void UInventoryWidget::CreateSlots()
{
	int index = 0;
	for (int i = 0; i < GridWidth; i++)
	{
		for (int p = 0; p < GridLength; p++)
		{
			UInventorySlotWidget* NewWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
			GridPanel->AddChildToUniformGrid(NewWidget, i, p);
			NewWidget->InitializeWidget(InventoryRef, index);
			index++;
		}
	}
}