#include "InteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Ludum58/CharacterControler/CharacterControler.h"
#include "Ludum58/Interface/InteractableInterface.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.1f; // 10 раз в секунду, как ты хотел
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        CachedCamera = Owner->FindComponentByClass<UCameraComponent>();
    }

    if (HoverWidgetClass && IsValid(GetWorld()))
    {
        ActiveWidget = CreateWidget<UUserWidget>(GetWorld(), HoverWidgetClass);
    }
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!CachedCamera) return;

    FVector Start = CachedCamera->GetComponentLocation();
    FVector End = Start + (CachedCamera->GetForwardVector() * TraceDistance);

    FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), true, GetOwner());
    FHitResult Hit;

    // Пускаем луч
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        AActor* HitActor = Hit.GetActor();
        
        if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
        {
            if (TargetActor != HitActor)
            {
                TargetActor = HitActor;
                ShowWidget(HitActor);
            }
            return;
        }
    }
    if (TargetActor)
    {
        HideWidget();
        TargetActor = nullptr;
    }
}

void UInteractionComponent::ShowWidget(AActor* Actor)
{
    if (ActiveWidget && !ActiveWidget->IsInViewport())
    {
        ActiveWidget->AddToViewport();
    }
}

void UInteractionComponent::HideWidget()
{
    if (ActiveWidget && ActiveWidget->IsInViewport())
    {
        ActiveWidget->RemoveFromParent();
    }
}

void UInteractionComponent::OnInteractPressed()
{
    if (!TargetActor) return;

    if (TargetActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
    {
        ACharacterControler* PlayerChar = Cast<ACharacterControler>(GetOwner());
        
        IInteractableInterface::Execute_OnInteract(TargetActor, PlayerChar, IdItem);
        
        HideWidget();
        TargetActor = nullptr;
    }
}