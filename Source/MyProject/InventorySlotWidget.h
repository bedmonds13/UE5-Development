// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "DragWidget.h"
#include "Item.h"
#include "Components/Image.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
class UInventoryComponent;
UCLASS()
class MYPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		UInventoryComponent* InventoryRef;

	UPROPERTY(BlueprintReadOnly)
		TSubclassOf<AItem> Item;

	UPROPERTY(BlueprintReadOnly)
		FString ItemName;

	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* ImageComponent;
	

	UPROPERTY(BlueprintReadOnly)
		FSlateBrush ItemImage;

	UPROPERTY(BlueprintReadOnly)
		int32 index;

	UPROPERTY(BlueprintReadOnly)
		bool SlotFilled;

	

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void InitializeWidget(UInventoryComponent* Inventory, int32 Newindex);
	void FillWidget(FString NewItemName, FSlateBrush NewItemImage);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	void RemoveItem();
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
};
