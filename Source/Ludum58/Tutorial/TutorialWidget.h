#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialSlideData.h"
#include "TutorialWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class LUDUM58_API UTutorialWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetSlides(const TArray<FTutorialSlideData>& NewSlides);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UImage* Image_Tutorial;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Tutorial;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Prev;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Next;

private:
    
    TArray<FTutorialSlideData> Slides;
    int32 CurrentSlideIndex = INDEX_NONE;

    UTexture2D* GetEffectiveImage(int32 Index) const;

    FText GetEffectiveText(int32 Index) const;

    void UpdateDisplay();

    // Переходит к указанному индексу, если он валиден
    void GoToSlide(int32 Index);

    UFUNCTION()
    void OnPrevClicked();

    UFUNCTION()
    void OnNextClicked();

    void CloseTutorial();
};