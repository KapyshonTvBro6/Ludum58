#pragma once
#include "CoreMinimal.h"
#include "InteractableInterface.generated.h"

class ACharacterControler;

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractableInterface
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(ACharacterControler* PlayerController, int32 IdItem);
};