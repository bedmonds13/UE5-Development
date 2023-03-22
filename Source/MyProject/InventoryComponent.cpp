#include "InventoryComponent.h"
#include "BaseCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"


UInventoryComponent::UInventoryComponent()
{
	InventoryItems.Reserve(20);
	DropDistanceMultiplier = 100;
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		PlayerControllerRef = Cast<APlayerController>(Character->GetController());	
		UInputComponent* PlayerInputComponent = Character->FindComponentByClass<UInputComponent>();
		if (PlayerInputComponent && PlayerControllerRef)
		{
				BindInput(PlayerInputComponent, PlayerControllerRef);
		}
	}	
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void UInventoryComponent::AddToInventory(AItem* NewItem)
{
	if (NewItem)
	{
		TSubclassOf<AItem> ItemClass = NewItem->GetClass();
		InventoryItems.Add(ItemClass);
		NewItem->Destroy();
	}
}

void UInventoryComponent::DropFromInventory(TSubclassOf<AItem> Item)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	FVector OutLocation =  Character->GetActorLocation() + (Character->GetActorForwardVector() * 100);
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(hitResult, OutLocation, OutLocation + FVector(0, 0, -10000), FCollisionObjectQueryParams());
	
	if (ShowDebugs)
	{
		DrawDebugSphere(GetWorld(), OutLocation, 8, 4, FColor::Red, false, 5.0f);
		DrawDebugLine(GetWorld(), OutLocation, hitResult.ImpactPoint, FColor::Red,false, 5.0f);
	}

	GetWorld()->SpawnActor<AItem>( Item, hitResult.ImpactPoint, FRotator::ZeroRotator);
	/*
	*/
}



void UInventoryComponent::BindInput(UInputComponent* PlayerInputComponent, APlayerController* PC)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(InventoryInputAction, ETriggerEvent::Triggered, this, &UInventoryComponent::ToggleInventory);
	
}

void UInventoryComponent::ToggleInventory()
{
	if (InventoryOpen)
	{
		if (InventoryWidget)
		{
			InventoryWidget->RemoveFromParent();
			InventoryWidget->MarkAsGarbage();
			InventoryWidget = nullptr;
		}
		InventoryOpen = false;

		if (PlayerControllerRef)
		{
			PlayerControllerRef->SetInputMode(FInputModeGameOnly());
			PlayerControllerRef->bShowMouseCursor = false;
		}
	}
	else
	{
		CreateInventoryWidget();
		InventoryOpen = true;

		if (PlayerControllerRef)
		{
			PlayerControllerRef->SetInputMode(FInputModeUIOnly());
			PlayerControllerRef->bShowMouseCursor = true;
		}
	}
}

void UInventoryComponent::CreateInventoryWidget()
{
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(),InventoryWidgetClass);
	
	if (InventoryWidget)
	{
		InventoryWidget->InitializeWidget(this);
		InventoryWidget->AddToViewport();
	}
}

TArray<TSubclassOf<AItem>> UInventoryComponent::GetInventoryItems()
{
	return InventoryItems;
}

void UInventoryComponent::SwapItems(int32 firstIndex, int32 secondIndex)
{
	InventoryItems.Swap(firstIndex, secondIndex);
}

void UInventoryComponent::ResetWidget()
{
	if (InventoryWidget)
	{
		InventoryWidget->Reset();
	}
}

void UInventoryComponent::MoveItem(int32 fromIndex, int32 toIndex)
{
	InventoryItems[toIndex] = InventoryItems[fromIndex];
	InventoryItems.RemoveAt(fromIndex);
}

void UInventoryComponent::RemoveIndex(int32 RemoveIndex)
{
	if (InventoryItems.IsValidIndex(RemoveIndex))
	{
		DropFromInventory(InventoryItems[RemoveIndex]);
		InventoryItems.RemoveAt(RemoveIndex);
	}
	if (InventoryOpen)
	{
		ResetWidget();
	}
}