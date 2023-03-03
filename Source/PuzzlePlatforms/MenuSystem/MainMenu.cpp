// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "MenuInterface.h"

bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success)
    {
        return false;
    }

    // Setup
    if (ensure(HostButton != nullptr))
    {
        HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
    }

    if (ensure(JoinButton != nullptr))
    {
        JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
    }

    if (ensure(CancelJoinMenuButton != nullptr))
    {
        CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
    }

    if (ensure(ConfirmJoinMenuButton != nullptr))
    {
        ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
    }

    if (ensure(QuitButton != nullptr))
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
    }

    return true;
}

void UMainMenu::HostServer()
{
    if (!ensure(MenuInterface != nullptr))
        return;

    MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
    if (!ensure(IPAddressField != nullptr))
        return;

    if (!ensure(MenuInterface != nullptr))
        return;

    MenuInterface->Join(IPAddressField->GetText().ToString());
}

void UMainMenu::OpenMainMenu()
{
    if (!ensure(MenuSwitcher != nullptr))
        return;

    if (!ensure(MainMenu != nullptr))
        return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenJoinMenu()
{
    if (!ensure(MenuSwitcher != nullptr))
        return;

    if (!ensure(JoinMenu != nullptr))
        return;

    MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::QuitGame()
{
    UWorld *World = GetWorld();
    if (!ensure(World != nullptr))
        return;

    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr))
        return;

    PlayerController->ConsoleCommand(TEXT("quit"), false);
}
