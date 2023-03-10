// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemType.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
