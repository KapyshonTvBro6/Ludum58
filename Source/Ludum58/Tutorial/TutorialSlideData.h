#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TutorialSlideData.generated.h"

USTRUCT(BlueprintType)
struct FTutorialSlideData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    UTexture2D* Image = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    FText Text;
};