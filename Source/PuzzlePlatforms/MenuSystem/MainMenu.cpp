// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ServerRow.h"
#include "MenuInterface.h"

UMainMenu::UMainMenu(const FObjectInitializer &ObjectInitializer)
{
    // Find server row Class widget BP classes
    static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
    if (!ensure(ServerRowBPClass.Class != nullptr))
    {
        return;
    }
    ServerRowClass = ServerRowBPClass.Class;
}

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

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
    if (!ensure(ServerRowClass != nullptr))
        return;

    if (!ensure(ServerList != nullptr))
        return;

    // Clear old serverRow
    ServerList->ClearChildren();

    // Create and add new row
    for (const FString &ServerName : ServerNames)
    {
        UServerRow *ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
        if (!ensure(ServerRow != nullptr))
            return;

        ServerRow->ServerName->SetText(FText::FromString(ServerName));
        ServerList->AddChild(ServerRow);
    }
}

void UMainMenu::JoinServer()
{
    if (!ensure(MenuInterface != nullptr))
        return;

    // TODO: Get server row selected
    MenuInterface->Join("");
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

    if (!ensure(MenuInterface != nullptr))
        return;

    // Request for refresh
    MenuInterface->RefreshServerList();
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
