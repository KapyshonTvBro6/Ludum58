#include "TutorialTrigger.h"
#include "Components/BoxComponent.h"
#include "TutorialWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

ATutorialTrigger::ATutorialTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetGenerateOverlapEvents(true);
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATutorialTrigger::BeginPlay()
{
    Super::BeginPlay();
    //Проверка наличия виджета и Целосности туториала
    if (!IsWigetClassWalid() || !IsFirstSlideComplete())
        ATutorialTrigger::Destroy();
    
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATutorialTrigger::OnTriggerBeginOverlap);
}

bool ATutorialTrigger::IsFirstSlideComplete() const
{
    if (Slides[0].Image && !Slides[0].Text.IsEmpty()) 
        return true;
    
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%s: Не полный слайд!"), *GetActorLabel()));
    UE_LOG(LogTemp, Error, TEXT("%s: Не полный слайд!"), *GetActorLabel());
    return false;
}

bool ATutorialTrigger::IsWigetClassWalid() const
{
    if (TutorialWidgetClass)
        return true;
    
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%s: Виджет не указан!"), *GetActorLabel()));
    UE_LOG(LogTemp, Error, TEXT("%s: Виджет не указан!"), *GetActorLabel());
    return false;
}

void ATutorialTrigger::OnTriggerBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (OtherActor != PlayerPawn)
        return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
        return;

    UTutorialWidget* TutorialWidget = CreateWidget<UTutorialWidget>(PC, TutorialWidgetClass);
    if (!TutorialWidget)
        return;

    TutorialWidget->SetSlides(Slides);

    TutorialWidget->AddToViewport(9999);

    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(TutorialWidget->TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);
    PC->SetShowMouseCursor(true);

    UGameplayStatics::SetGamePaused(GetWorld(), true);
    ATutorialTrigger::Destroy();
}