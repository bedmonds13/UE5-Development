#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "InventoryWidget.h"
#include "InputAction.h"
#include "InventoryComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	bool InventoryOpen;
	UInventoryWidget* InventoryWidget;

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintReadOnly)
		TArray<TSubclassOf<AItem>> InventoryItems;

	UPROPERTY(EditDefaultsOnly)
			UInputAction* InventoryInputAction;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	APlayerController* PlayerControllerRef;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AddToInventory(AItem* NewItem);
	void RemoveItem(AItem* RemoveItem);
	void RemoveIndex(int32 RemoveIndex);
	void BindInput(UInputComponent* PlayerInputComponent, APlayerController* PC);
	UFUNCTION()
	void ToggleInventory();
	void CreateInventoryWidget();
	void ResetWidget();
	TArray<TSubclassOf<AItem>> GetInventoryItems();
	void SwapItems(int32 firstIndex, int32 secondIndex);
	void MoveItem(int32 fromIndex, int32 toIndex);
};