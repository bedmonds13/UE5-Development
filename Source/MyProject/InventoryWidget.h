// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragWidget.h"
#include "Input/Reply.h"
#include "Components/UniformGridPanel.h"
#include "InventorySlotWidget.h"
#include "Widgets/InventoryCloseWidget.h"
#include "Components/Button.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class UInventoryComponent;
UCLASS()
class MYPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
		
	UPROPERTY(EditDefaultsOnly)
		int32 GridWidth;
	UPROPERTY(EditDefaultsOnly)
		int32 GridLength;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UUniformGridPanel* GridPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* CloseButton;
	
public:
	void InitializeWidget(UInventoryComponent* InventoryItems);
	void Reset();

private:
	void FillSlots();
	void CreateSlots();
	void ClearSlots();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UInventoryComponent* InventoryRef;
};
