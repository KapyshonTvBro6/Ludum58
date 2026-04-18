#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialSlideData.h"
#include "TutorialTrigger.generated.h"

class UBoxComponent;
class UTutorialWidget;

UCLASS()
class LUDUM58_API ATutorialTrigger : public AActor
{
    GENERATED_BODY()

public:
    ATutorialTrigger();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial")
    TArray<FTutorialSlideData> Slides;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UTutorialWidget> TutorialWidgetClass;

    UFUNCTION()
    void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

private:
    // Проверка на целосность первого слайда
    bool IsFirstSlideComplete() const;
    // Проверка на наличие виджита
    bool IsWigetClassWalid() const;
};