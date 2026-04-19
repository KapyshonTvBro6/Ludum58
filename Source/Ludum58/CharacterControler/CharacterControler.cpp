#include "CharacterControler.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Ludum58/UI/PauseMenuWidget.h"
#include "Ludum58/Interface/InteractableInterface.h"

ACharacterControler::ACharacterControler()
{
    PrimaryActorTick.bCanEverTick = true;

    // Настройка камеры от 1-го лица
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 0.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FirstPersonCamera->bUsePawnControlRotation = true;

    // Настройка движения
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->JumpZVelocity = jumpStrong;
    GetCharacterMovement()->AirControl = mousSpeed;
    GetCharacterMovement()->MaxWalkSpeed = wolkSpeed;
    
    InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp"));
}

void ACharacterControler::BeginPlay()
{
    Super::BeginPlay();

    // Добавляем Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

void ACharacterControler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Проверяем, что это Enhanced Input Component
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Привязка действий
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterControler::Move);
        }
        
        if (PauseAction)
        {
            EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ACharacterControler::TogglePause);
        }
        
        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterControler::Look);
        }

        if (JumpAction)
        {
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }

        if (InteractAction)
        {
            EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACharacterControler::Interact);
        }
    }
}

void ACharacterControler::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Движение вперед/назад
        if (MovementVector.Y != 0.0f)
        {
            AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        }

        // Движение влево/вправо
        if (MovementVector.X != 0.0f)
        {
            AddMovementInput(GetActorRightVector(), MovementVector.X);
        }
    }
}

void ACharacterControler::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Добавляем вращение по Y (поворот) и X (наклон головы)
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y * -1);
    }
}

void ACharacterControler::Interact(const FInputActionValue& Value)
{
    TraceForInteraction();
}

void ACharacterControler::TraceForInteraction()
{
    FVector Start = FirstPersonCamera->GetComponentLocation();
    FVector End = Start + (FirstPersonCamera->GetForwardVector() * 250.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionTrace), true, this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
        {
            // 🦊 ИСПРАВЛЕНИЕ: Передаем 3 аргумента
            // 1. TargetActor (на кого попали)
            // 2. PlayerController (this - кто нажал)
            // 3. IdItem (1 - какой ID передать, например, для теста)
            IInteractableInterface::Execute_OnInteract(HitActor, this, 0);
            
            UE_LOG(LogTemp, Log, TEXT("🔔 Взаимодействие с: %s (ID: 1)"), *HitActor->GetName());
        }
    }
}

void ACharacterControler::TogglePause(const FInputActionValue& Value)
{
    bIsPaused = !bIsPaused;
    UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);

    APlayerController* PC = GetController<APlayerController>();
    if (PC)
    {
        if (bIsPaused)
        {
            // Показываем меню паузы
            ShowPauseMenu();
            PC->bShowMouseCursor = true;
            PC->SetInputMode(FInputModeUIOnly());
        }
        else
        {
            // Скрываем меню паузы
            HidePauseMenu();
            PC->bShowMouseCursor = false;
            PC->SetInputMode(FInputModeGameOnly());
        }
    }
}

void ACharacterControler::ShowPauseMenu()
{
    if (!PauseMenuWidget && PauseMenuWidgetClass)
    {
        PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
    }

    if (PauseMenuWidget && !PauseMenuWidget->IsInViewport())
    {
        PauseMenuWidget->AddToViewport();
    }
}

void ACharacterControler::HidePauseMenu()
{
    if (PauseMenuWidget && PauseMenuWidget->IsInViewport())
    {
        PauseMenuWidget->RemoveFromParent();
    }
}