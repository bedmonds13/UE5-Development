// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttackInputComponent.h"
#include "BaseCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UCharacterAttackInputComponent::UCharacterAttackInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InputEnabled = true;
	// ...
}


// Called when the game starts
void UCharacterAttackInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//
	
	
	// ...
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (UInputComponent* PlayerInputComponent = Character->FindComponentByClass<UInputComponent>() )
		{
			if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
			{
				BindInput(PlayerInputComponent, PC);
			}
		}
	}
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Cast<ABaseCharacter>(GetOwner())->GetMesh()->GetAnimInstance());
	AnimInstance->OnMontageEnded.AddDynamic(this, &UCharacterAttackInputComponent::ResetCombo_Delay);
	
}


// Called every frame
void UCharacterAttackInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterAttackInputComponent::BindInput(UInputComponent* PlayerInputComponent, APlayerController* PC)
{
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(LightAttackCombo.InputAction, ETriggerEvent::Triggered, this, &UCharacterAttackInputComponent::LightAttack);
	PEI->BindAction(HeavyAttackCombo.InputAction, ETriggerEvent::Triggered, this, &UCharacterAttackInputComponent::HeavyAttack);
	
}

void UCharacterAttackInputComponent::LightAttack(const FInputActionValue& Value)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (InputEnabled)
		{
			if (CurrentComboConfig == nullptr || CurrentComboConfig == &LightAttackCombo)
			{
				CurrentComboConfig = &LightAttackCombo;
				if (current_index <= LightAttackCombo.AnimMontages.Num() - 1 &&  LightAttackCombo.AnimMontages[current_index] != nullptr)
				{
					float TimerLength = LightAttackCombo.AnimMontages[current_index]->GetPlayLength() * 0.75f;
					GetWorld()->GetTimerManager().SetTimer(ComboInputDelay, this, &UCharacterAttackInputComponent::HandleComboDelay, TimerLength, false);
					CurrentMontage = LightAttackCombo.AnimMontages[current_index];
					
					Character->PlayAnimMontage(LightAttackCombo.AnimMontages[current_index]);
					current_index++;
				}
				else
					ResetCombo();
			}
			else
				ResetCombo();
		}
	}
}

void UCharacterAttackInputComponent::HeavyAttack(const FInputActionValue& Value)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (InputEnabled)
		{
			if (CurrentComboConfig == nullptr || CurrentComboConfig == &HeavyAttackCombo )
			{
				CurrentComboConfig = &HeavyAttackCombo;
				if (current_index <= HeavyAttackCombo.AnimMontages.Num() - 1 && HeavyAttackCombo.AnimMontages[current_index] != nullptr)
				{
					float TimerLength = HeavyAttackCombo.AnimMontages[current_index]->GetPlayLength() * 0.75f;
					GetWorld()->GetTimerManager().SetTimer(ComboInputDelay, this, &UCharacterAttackInputComponent::HandleComboDelay, TimerLength, false);
					Character->PlayAnimMontage(HeavyAttackCombo.AnimMontages[current_index]);
					current_index++;
				}
				else
					ResetCombo();
			}
			else
				ResetCombo();			
		}
	}
}


void UCharacterAttackInputComponent::ResetCombo()
{
	CurrentComboConfig = nullptr;
	current_index = 0;
	InputEnabled = true;
}

void UCharacterAttackInputComponent::ResetCombo_Delay(UAnimMontage* Montage, bool Interrupted)
{
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &UCharacterAttackInputComponent::ResetCombo, 3.0f, false);
}

void UCharacterAttackInputComponent::HandleComboDelay()
{
	InputEnabled = true;
}
