#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LUDUM58_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void OnInteractPressed();

protected:
    virtual void BeginPlay() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Config", meta = (ClampMin = "0"))
    int32 IdItem = 0;

private:
    UPROPERTY(EditAnywhere, Category = "Interaction|Config")
    TSubclassOf<class UUserWidget> HoverWidgetClass;

    UPROPERTY(EditAnywhere, Category = "Interaction|Config")
    float TraceDistance = 250.0f;

    UPROPERTY(VisibleAnywhere, Category = "Interaction|Debug")
    class UCameraComponent* CachedCamera = nullptr;

    UPROPERTY()
    class UUserWidget* ActiveWidget = nullptr;

    UPROPERTY()
    AActor* TargetActor = nullptr;

    void ShowWidget(AActor* Actor);
    void HideWidget();
};