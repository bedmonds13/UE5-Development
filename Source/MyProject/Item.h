// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemType.h"
#include "Styling/SlateBrush.h"
#include "Components/BoxComponent.h"
#include "Item.generated.h"

UCLASS()
class MYPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Type)
		TEnumAsByte<ItemType> Type;
	
public:	
	// Sets default values for this actor's properties
	AItem();
	
	UPROPERTY(EditDefaultsOnly)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly)
	FSlateBrush ItemImage;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* BoxComponent;

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
