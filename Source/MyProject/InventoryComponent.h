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
	APlayerController* PlayerControllerRef;

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintReadOnly)
		TArray<TSubclassOf<AItem>> InventoryItems;

	UPROPERTY(EditDefaultsOnly)
			UInputAction* InventoryInputAction;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BindInput(UInputComponent* PlayerInputComponent, APlayerController* PC);
	
	void CreateInventoryWidget();
	
	UFUNCTION()
	void ToggleInventory();
	
	UFUNCTION(BlueprintCallable)
	void AddToInventory(AItem* NewItem);
	
	
	UFUNCTION(BlueprintCallable)
	void RemoveIndex(int32 RemoveIndex);

	void SwapItems(int32 firstIndex, int32 secondIndex);

	
	void ResetWidget();
	
	TArray<TSubclassOf<AItem>> GetInventoryItems();
	
	void MoveItem(int32 fromIndex, int32 toIndex);
};