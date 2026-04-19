#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InteractionComponent.h"
#include "CharacterControler.generated.h"

UCLASS()
class LUDUM58_API ACharacterControler : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterControler();

	void TogglePause(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Компоненты камеры
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCamera;

	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* InteractAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UInteractionComponent* InteractionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Specifications")
	float mousSpeed = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Specifications")
	float wolkSpeed = 600.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Specifications")
	float jumpStrong = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* PauseAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	bool bIsPaused = false;
	
	UPROPERTY()
	class UPauseMenuWidget* PauseMenuWidget;

	void ShowPauseMenu();
	void HidePauseMenu();
	
private:
	// Движение
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Взаимодействие
	void Interact(const FInputActionValue& Value);
	void TraceForInteraction();
};