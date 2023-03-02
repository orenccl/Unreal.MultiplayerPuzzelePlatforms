// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "MenuInterface.h"

bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success)
    {
        return false;
    }

    if (!ensure(Host != nullptr) || !ensure(Join != nullptr))
    {
        return false;
    }

    // Setup
    Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
    Join->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface *Interface)
{
    MenuInterface = Interface;
}

void UMainMenu::Setup()
{
    // Display on screen
    this->AddToViewport();

    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
        return;

    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr))
        return;

    // Focus widget and show mouse
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);
    PlayerController->SetShowMouseCursor(true);
}

void UMainMenu::Teardown()
{
    this->RemoveFromViewport();

    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
        return;

    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr))
        return;

    // Hide mouse
    FInputModeGameOnly InputModeData;
    PlayerController->SetInputMode(InputModeData);
    PlayerController->SetShowMouseCursor(false);
}

void UMainMenu::HostServer()
{
    if (!ensure(MenuInterface != nullptr))
        return;

    MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
    UE_LOG(LogTemp, Warning, TEXT("I'm gonna join a server!"));
}
