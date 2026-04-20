#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractableItem.generated.h"

UCLASS()
class LUDUM58_API AInteractableItem : public AActor, public IInteractableInterface
{
    GENERATED_BODY()

public:
    AInteractableItem();
    
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractMe(int32 IdItem);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* MeshComp;

    UPROPERTY(EditAnywhere, Category = "Interaction|Switch")
    UStaticMesh* MeshFor;

    UPROPERTY(EditAnywhere, Category = "Interaction|Decal")
    class UMaterialInterface* DecalMaterial1;

    UPROPERTY(EditAnywhere, Category = "Interaction|Decal")
    class UMaterialInterface* DecalMaterial2;

    UPROPERTY(EditAnywhere, Category = "Interaction|UI")
    TSubclassOf<class UUserWidget> FallbackWidgetClass;

private:
    void SpawnDecal(UMaterialInterface* Material);
    void ShowFallbackWidget();
};