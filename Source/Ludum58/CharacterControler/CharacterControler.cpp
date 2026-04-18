#include "CharacterControler.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
    
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
        AddControllerYawInput(LookAxisVector.X * mousSpeed);
        AddControllerPitchInput(LookAxisVector.Y * -mousSpeed);
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
            IInteractableInterface::Execute_OnInteract(HitActor);
            UE_LOG(LogTemp, Log, TEXT("Взаимодействие с: %s"), *HitActor->GetName());
        }
    }
}