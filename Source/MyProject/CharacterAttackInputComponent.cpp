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
	UseDebugs = false;
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
	if (CurrentComboConfig == nullptr)
	{
		CurrentComboConfig = &LightAttackCombo;
		Attack();
	}
	else if (!IsComboInterrupted(LightAttackCombo))
		Attack();
	else
		ResetCombo();
}

void UCharacterAttackInputComponent::HeavyAttack(const FInputActionValue& Value)
{
	if (CurrentComboConfig == nullptr)
	{
		CurrentComboConfig = &HeavyAttackCombo;
		Attack();
	}
	else if (!IsComboInterrupted(HeavyAttackCombo))
		Attack();
	else
		ResetCombo();
}


void UCharacterAttackInputComponent::ResetCombo()
{
	CurrentComboConfig = nullptr;
	current_index = 0;
	InputEnabled = true;
	CurrentMontage = nullptr;
}



bool UCharacterAttackInputComponent::IsComboInterrupted(FCharacterCombo& Combo)
{
	return  !(CurrentComboConfig == &Combo);
}



void UCharacterAttackInputComponent::Attack()
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (InputEnabled)
		{
			DebugMessage(FString("Input enabled."));
			if (CurrentComboConfig != nullptr )
			{
				if (current_index <= CurrentComboConfig->CharacterAttack.Num() - 1 && CurrentComboConfig->CharacterAttack[current_index].AttackMontage != nullptr)
				{
					if (CurrentMontage && Character->GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage))
					{
						DebugMessage(FString("Montage is still in play."));
						float timeElapsed = Character->GetMesh()->GetAnimInstance()->Montage_GetPosition(CurrentMontage);
						
						float timeLeft = (CurrentMontage->GetPlayLength()* 0.75f) -  timeElapsed;
						
						if(timeLeft > 0)
							GetWorld()->GetTimerManager().SetTimer(PlayMontageDelay, this, &UCharacterAttackInputComponent::PlayAttackMontage, timeLeft, false);
						else
							Character->PlayAnimMontage(CurrentComboConfig->CharacterAttack[current_index].AttackMontage);
					}
					else
					{
						DebugMessage(FString("Previous montage not in play."));
						Character->PlayAnimMontage(CurrentComboConfig->CharacterAttack[current_index].AttackMontage);
					}
					CurrentMontage = CurrentComboConfig->CharacterAttack[current_index].AttackMontage;
					InputEnabled = false;
					if (current_index == CurrentComboConfig->CharacterAttack.Num() - 1)
					{
						DebugMessage(FString("Playing last montage in Attack Combo"));
						float TimerLength = CurrentComboConfig->CharacterAttack[current_index].AttackMontage->GetPlayLength();
						GetWorld()->GetTimerManager().SetTimer(ComboInputDelay, this, &UCharacterAttackInputComponent::ResetCombo, TimerLength, false);
					}
					else
					{
						float TimerLength = CurrentComboConfig->CharacterAttack[current_index].AttackMontage->GetPlayLength() * 0.25f;
						GetWorld()->GetTimerManager().SetTimer(ComboInputDelay, this, &UCharacterAttackInputComponent::HandleComboDelay, TimerLength, false);

					}
					current_index++;
				}
				else
					ResetCombo();
			}
			DebugMessage(FString("Input disabled."));
		}
	}
}

void UCharacterAttackInputComponent::ResetCombo_Delay(UAnimMontage* Montage, bool Interrupted)
{
	if (CurrentComboConfig)
	{
		if (CurrentComboConfig->CharacterAttack[current_index - 1].AttackMontage != nullptr)
		{
			FCharacterAttack CharacterAttack = CurrentComboConfig->CharacterAttack[current_index - 1];
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &UCharacterAttackInputComponent::ResetCombo, CharacterAttack.InputDelay, false);
		}
	}
	else
		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &UCharacterAttackInputComponent::ResetCombo, 1.0f, false);
}

void UCharacterAttackInputComponent::PlayAttackMontage()
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (CurrentMontage)
			Character->PlayAnimMontage(CurrentMontage);
	}
}

void UCharacterAttackInputComponent::HandleComboDelay()
{
	InputEnabled = true;
}

void UCharacterAttackInputComponent::DebugMessage(FString Message)
{
	if (GEngine && UseDebugs)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Message, false);
}

