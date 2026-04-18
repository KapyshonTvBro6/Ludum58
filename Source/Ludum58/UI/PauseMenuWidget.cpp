#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Ludum58/CharacterControler/CharacterControler.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetupButtonBindings();
}

void UPauseMenuWidget::SetupButtonBindings()
{
    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeButtonClicked);
    }
}

void UPauseMenuWidget::OnResumeButtonClicked()
{
    ACharacterControler* PlayerCharacter = Cast<ACharacterControler>(GetOwningPlayerPawn());
    if (PlayerCharacter)
    {
        PlayerCharacter->TogglePause(FInputActionValue(1.0f));
    }
}