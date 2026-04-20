#include "InteractableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Ludum58/CharacterControler/CharacterControler.h"

AInteractableItem::AInteractableItem()
{
    PrimaryActorTick.bCanEverTick = false;
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    MeshComp->SetCollisionProfileName(TEXT("OverlapAll"));
}

void AInteractableItem::InteractMe(int32 IdItem)
{
    UE_LOG(LogTemp, Warning, TEXT("🦊 InteractMe вызван! ID: %d"), IdItem);
    switch (IdItem)
    {
    case 1:
        if (MeshFor) MeshComp->SetStaticMesh(MeshFor);
        SpawnDecal(DecalMaterial1);
        break;

    case 2:
        if (MeshFor) MeshComp->SetStaticMesh(MeshFor);
        SpawnDecal(DecalMaterial2);
        break;

    default:
        ShowFallbackWidget();
        break;
    }
}

void AInteractableItem::SpawnDecal(UMaterialInterface* Material)
{
    if (!Material || !MeshComp) return;
    FVector SpawnLoc = MeshComp->GetComponentLocation() + FVector(0.f, 0.f, 60.f);
    UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
        GetWorld(), Material, FVector(80.f), SpawnLoc, FRotator::ZeroRotator, 5.0f);
    
    if (Decal) Decal->SetFadeOut(2.0f, 1.5f);
}

void AInteractableItem::ShowFallbackWidget()
{
    if (FallbackWidgetClass && IsValid(GetWorld()))
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), FallbackWidgetClass);
        if (Widget) Widget->AddToViewport();
    }
}