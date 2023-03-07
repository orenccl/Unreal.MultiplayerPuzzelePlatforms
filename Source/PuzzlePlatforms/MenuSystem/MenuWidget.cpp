// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::Setup()
{
    // Set can be focus and interact with input
    bIsFocusable = true;

    AddToViewport();

    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
        return;

    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr))
        return;

    // InputMode use FInputModeUIOnly to prevent other input interaction
    FInputModeUIOnly InputModeData;
    // Focus on widget self
    InputModeData.SetWidgetToFocus(TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);
    PlayerController->SetShowMouseCursor(true);
}

void UMenuWidget::SetMenuInterface(IMenuInterface *Interface)
{
    MenuInterface = Interface;
}

void UMenuWidget::NativeDestruct()
{
    Super::NativeDestruct();

    UWorld *World = GetWorld();
    if (!World)
        return;

    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (!PlayerController)
        return;

    RemoveFromParent();
    // InputMode change back to FInputModeGameOnly, not interact with UI anymore
    FInputModeGameOnly InputModeData;
    PlayerController->SetInputMode(InputModeData);
    PlayerController->SetShowMouseCursor(false);
}
