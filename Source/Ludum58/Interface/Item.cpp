#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "Ludum58/CharacterControler/InteractionComponent.h"
#include "Ludum58/CharacterControler/CharacterControler.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    MeshComp->SetCollisionProfileName(TEXT("OverlapAll"));

    InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp"));
}

void AItem::OnInteract_Implementation(ACharacterControler* PlayerController, int32 PlayerHeldId)
{
    if (!PlayerController) return;

    UE_LOG(LogTemp, Warning, TEXT(" [AItem] Руки: %d | Предмет: %d"), 
           PlayerController->CurrentInteractionId, IdItem);

    // ✅ ЕСЛИ РУКИ ПУСТЫЕ (0) -> ЗАБИРАЕМ ПРЕДМЕТ
    if (PlayerController->CurrentInteractionId == 0)
    {
        PlayerController->CurrentInteractionId = IdItem;
        UE_LOG(LogTemp, Warning, TEXT("✅ Подобрали! Новый ID рук: %d"), IdItem);
        
        // Предмет "уходит" в инвентарь (скрываем с уровня)
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
    }
    // ✅ ЕСЛИ ДЕРЖИМ ЭТОТ ЖЕ ПРЕДМЕТ -> КЛАДЁМ ОБРАТНО
    else if (PlayerController->CurrentInteractionId == IdItem)
    {
        PlayerController->CurrentInteractionId = 0;
        SetActorHiddenInGame(false);
        SetActorEnableCollision(true);
        UE_LOG(LogTemp, Warning, TEXT(" Положили обратно. ID рук: 0"));
    }
    // ⚠️ ЗАНЯТО ДРУГИМ ПРЕДМЕТОМ
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Руки заняты другим предметом (ID: %d)"), 
               PlayerController->CurrentInteractionId);
    }
}

void AItem::OnItemIdChanged_Implementation(int32 OldId, int32 NewId)
{
    // По умолчанию ничего не делаем, но Blueprint может переопределить
    UE_LOG(LogTemp, Log, TEXT("🔄 Item ID changed: %d -> %d"), OldId, NewId);
}