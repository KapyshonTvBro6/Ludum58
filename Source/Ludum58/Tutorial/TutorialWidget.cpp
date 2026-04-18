#include "TutorialWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UTutorialWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Prev)
    {
        Button_Prev->OnClicked.AddDynamic(this, &UTutorialWidget::OnPrevClicked);
    }
    if (Button_Next)
    {
        Button_Next->OnClicked.AddDynamic(this, &UTutorialWidget::OnNextClicked);
    }
}

void UTutorialWidget::SetSlides(const TArray<FTutorialSlideData>& NewSlides)
{
    Slides = NewSlides;
    CurrentSlideIndex = 0;
    
    GoToSlide(CurrentSlideIndex);
}

UTexture2D* UTutorialWidget::GetEffectiveImage(int32 Index) const
{
    for (int32 i = Index; i >= 0; i--)
    {
        if (Slides[i].Image)
            return Slides[i].Image;
    }
    return nullptr;
}

FText UTutorialWidget::GetEffectiveText(int32 Index) const
{
    for (int32 i = Index; i >= 0; i--)
    {
        if (!Slides[i].Text.IsEmpty())
            return Slides[i].Text;
    }
    FText Result;
    return Result;
}

void UTutorialWidget::GoToSlide(int32 Index)
{    
    CurrentSlideIndex = Index;
    UpdateDisplay();

    if (Button_Prev)
        Button_Prev->SetIsEnabled(CurrentSlideIndex > 0);
}

void UTutorialWidget::UpdateDisplay()
{
    Image_Tutorial->SetBrushFromTexture(GetEffectiveImage(CurrentSlideIndex));
    Text_Tutorial->SetText(GetEffectiveText(CurrentSlideIndex));
}

void UTutorialWidget::OnPrevClicked()
{
    if (CurrentSlideIndex > 0)
    {
        GoToSlide(CurrentSlideIndex - 1);
    }
}

void UTutorialWidget::OnNextClicked()
{
    if (CurrentSlideIndex < Slides.Num() - 1)
    {
        GoToSlide(CurrentSlideIndex + 1);
    }
    else
    {
        CloseTutorial();
    }
}

void UTutorialWidget::CloseTutorial()
{
    RemoveFromParent();

    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->SetShowMouseCursor(false);
    }

    UGameplayStatics::SetGamePaused(GetWorld(), false);
}