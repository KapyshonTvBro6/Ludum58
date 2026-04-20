#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "Item.generated.h"

class ACharacterControler;

UCLASS(Abstract, Blueprintable)
class LUDUM58_API AItem : public AActor, public IInteractableInterface
{
    GENERATED_BODY()

public:
    AItem();

    // Реализация интерфейса
    virtual void OnInteract_Implementation(ACharacterControler* PlayerController, int32 IdItem) override;

    UFUNCTION(BlueprintPure, Category = "Item|ID")
    int32 GetItemId() const { return IdItem; }

protected:
    // Текущий ID предмета
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Config", meta = (ClampMin = "0"))
    int32 IdItem = 1;

    // Визуальный компонент (меш)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* MeshComp;

    // Компонент взаимодействия (для передачи ID)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UInteractionComponent* InteractionComp;

    // Событие при изменении ID (можно переопределить в BP)
    UFUNCTION(BlueprintNativeEvent, Category = "Item|Events")
    void OnItemIdChanged(int32 OldId, int32 NewId);
    virtual void OnItemIdChanged_Implementation(int32 OldId, int32 NewId);

    // Виртуальный метод для кастомной логики взаимодействия
    UFUNCTION(BlueprintNativeEvent, Category = "Item|Interaction")
    void HandleInteraction(ACharacterControler* PlayerController);
    virtual void HandleInteraction_Implementation(ACharacterControler* PlayerController) {}
};