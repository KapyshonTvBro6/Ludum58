#include "InteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Ludum58/Interface/InteractableInterface.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.1f; // 10 раз в секунду, как ты хотел
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    // Ищем камеру у владельца (не зависит от имени класса персонажа)
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

    if (!CachedCamera || !IsValid(GetWorld())) return;

    FVector Start = CachedCamera->GetComponentLocation();
    FVector End = Start + (CachedCamera->GetForwardVector() * TraceDistance);

    FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), true, GetOwner());
    FHitResult Hit;

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            if (HitActor != TargetActor)
            {
                TargetActor = HitActor;
                ShowWidget(HitActor);
            }
            return;
        }
    }

    // Луч не попал ни во что, а объект был выбран
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
    if (TargetActor && TargetActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
    {
        IInteractableInterface::Execute_OnInteract(TargetActor);
        HideWidget();
        TargetActor = nullptr;
    }
}