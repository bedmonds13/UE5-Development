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

protected:
	UPROPERTY(BlueprintReadOnly)
		FString ItemName;

	UPROPERTY(BlueprintReadOnly)
		TSubclassOf<AItem> Item;

	UPROPERTY(BlueprintReadOnly)
		UInventoryComponent* InventoryRef;



	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* ImageComponent;
	

	UPROPERTY(BlueprintReadOnly)
		FSlateBrush ItemImage;

	UPROPERTY(BlueprintReadOnly)
		int32 index;

	UPROPERTY(BlueprintReadOnly)
		bool SlotFilled;


	

public:
	void InitializeWidget(UInventoryComponent* Inventory, int32 Newindex);
	void RemoveItem();
	void FillWidget(FString NewItemName, FSlateBrush NewItemImage);


private:
	//Drag Operations
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
