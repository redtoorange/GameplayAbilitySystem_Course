// Copyright Andrew McGuiness 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class IEnemyInterface;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputMappingContext> AuraContext;

	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	
	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	IEnemyInterface* LastHoveredActor;
	IEnemyInterface* CurrentHoveredActor;
	
protected:
	virtual void SetupInputComponent() override;

public:
	virtual void PlayerTick(float DeltaTime) override;
};
