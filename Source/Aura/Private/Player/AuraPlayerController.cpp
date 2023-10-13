// Copyright Andrew McGuiness 2023


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Halts execution if this pointer is not set
	check(AuraContext);

	// We need to get the local player's InputSubsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 1);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Fetch the InputComponent from the PlayerController and cast it to Enhanced so we can use the new system
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Bind an Action to an event, this is the same as the legacy input binding
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Pull the value from the input Actio
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// Flatten the input and calculate direction components
	const FRotator Rotation{0, GetControlRotation().Yaw, 0};
	const FVector Forward = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	// If there is a pawn, add the movement input to the pawn
	if (APawn* controlledPawn = GetPawn<APawn>())
	{
		// In this Aura case, the pawn will be a Character with a CharacterMovementComponent
		controlledPawn->AddMovementInput(Forward, InputAxisVector.Y);
		controlledPawn->AddMovementInput(Right, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastHoveredActor = CurrentHoveredActor;
	CurrentHoveredActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastHoveredActor != nullptr && CurrentHoveredActor == nullptr)
	{
		LastHoveredActor->UnHighlightActor();
	}
	else if (LastHoveredActor == nullptr && CurrentHoveredActor != nullptr)
	{
		CurrentHoveredActor->HighLightActor();
	}
	else if (LastHoveredActor != nullptr && CurrentHoveredActor != nullptr)
	{
		if (LastHoveredActor != CurrentHoveredActor)
		{
			LastHoveredActor->UnHighlightActor();
			CurrentHoveredActor->HighLightActor();
		}
	}
}
