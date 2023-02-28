// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "MyInputConfigData.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CanAttack = true;
	comboindex_previous = -1;
	comboindex_next = 0;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* PC = Cast <APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);


	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
	PEI->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
	PEI->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	PEI->BindAction(InputActions->InputJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//Combo Input
	//PEI->BindAction(InputActions->InputLightAttack, ETriggerEvent::Triggered, this, &ABaseCharacter::LightAttack);
	//PEI->BindAction(InputActions->InputHeavyAttack, ETriggerEvent::Triggered, this, &ABaseCharacter::HeavyAttack);
	PEI->BindAction(InputActions->InputComboAttack, ETriggerEvent::Completed, this, &ABaseCharacter::ComboAttack);
	PEI->BindAction(InputSequenceAttack, ETriggerEvent::Triggered, this, &ABaseCharacter::SequenceAttack);
	
	
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		// Forward/Backward direction
		if (MoveValue.Y != 0.f)
		{
			// Get forward vector
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			float moveValue_Y = MoveValue.Y;
			AddMovementInput(Direction, moveValue_Y);
		}

		// Right/Left direction
		if (MoveValue.X != 0.f)
		{
			// Get right vector
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();

		if (LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
	}
}

void ABaseCharacter::CheckCombo(UAnimMontage* NextMontage)
{
	if (NextMontage)
	{
		if (comboindex_previous < comboindex_next)
		{
			comboindex_previous = comboindex_next;
			PlayAnimMontage(NextMontage);
		}
		else
			ResetCombo();
	}
	else
		ResetCombo();
}

void ABaseCharacter::LightAttack(const FInputActionValue& Value)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString("Light Attack!"), false);

	if (LightAttackAnim && CanAttack)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::Event_OnMontageEnded);
		PlayAnimMontage(LightAttackAnim);
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			AActor::DisableInput(PlayerController);
		CanAttack = false;
		SetInputDisableTimer(LightAttackAnim->GetPlayLength());
	}

}
void ABaseCharacter::ResetCombo()
{
	comboindex_previous = -1;
	comboindex_next = 0;
}
void ABaseCharacter::Event_OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
void ABaseCharacter::HeavyAttack(const FInputActionValue& Value)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString("Heavy Attack!"), false);
	if (HeavyAttackAnim && CanAttack)
	{
		PlayAnimMontage(HeavyAttackAnim);
		if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			AActor::DisableInput(PlayerController);
		CanAttack = false;
		SetInputDisableTimer(HeavyAttackAnim->GetPlayLength());
	}
}

void ABaseCharacter::ComboAttack(const FInputActionValue& Value)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString("Completed Standard Combo Attack!"), false);
}

void ABaseCharacter::SequenceAttack(const FInputActionValue& Value)
{
	if (comboindex_previous == -1 && comboindex_next == 0)
	{
		PlayAnimMontage(SequenceAttackAnim);
		comboindex_previous = 0;
		return;
		
	}
	else if (comboindex_previous < comboindex_next)
	{
		if (comboindex_next - comboindex_previous == 1)
		{
			return;
		}
	}
	else
	{
		comboindex_next++;
	}
}

void ABaseCharacter::SetInputDisableTimer(float TimerRate)
{
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseCharacter::EnablePlayerInput,  TimerRate, false);
}

void ABaseCharacter::EnablePlayerInput()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (PlayerController)
	{
		AActor::EnableInput(PlayerController);
		CanAttack = true;
	}	


}

