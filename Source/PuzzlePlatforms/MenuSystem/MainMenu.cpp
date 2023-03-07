// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
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
        HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
    }

    if (ensure(JoinButton != nullptr))
    {
        JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
    }

    if (ensure(CancelHostMenuButton != nullptr))
    {
        CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
    }

    if (ensure(ConfirmHostMenuButton != nullptr))
    {
        ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
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

    if (!ensure(ServerName != nullptr))
        return;

    MenuInterface->Host(ServerName->GetText().ToString());
}

void UMainMenu::SetServerList(TArray<FServerData> ServerDatas)
{
    if (!ensure(ServerRowClass != nullptr))
        return;

    if (!ensure(ServerList != nullptr))
        return;

    // Clear old serverRow
    ServerList->ClearChildren();

    for (size_t i = 0; i < ServerDatas.Num(); i++)
    {
        UServerRow *ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
        if (!ensure(ServerRow != nullptr))
            return;

        ServerRow->Setup(this, i);
        ServerRow->ServerName->SetText(FText::FromString(ServerDatas[i].Name));
        ServerRow->HostUser->SetText(FText::FromString(ServerDatas[i].HostUserName));
        ServerRow->ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), ServerDatas[i].CurrentPlayers, ServerDatas[i].MaxPlayers)));
        ServerList->AddChild(ServerRow);
    }
}

void UMainMenu::SelectIndex(uint32 Index)
{
    SelectedIndex = Index;
    UpdateServerListChildren();
}

void UMainMenu::JoinServer()
{
    if (!ensure(MenuInterface != nullptr))
        return;

    if (!SelectedIndex.IsSet())
    {
        UE_LOG(LogTemp, Warning, TEXT("SelectedIndex not set"));
        return;
    }

    MenuInterface->Join(SelectedIndex.GetValue());
}

void UMainMenu::OpenMainMenu()
{
    if (!ensure(MenuSwitcher != nullptr))
        return;

    if (!ensure(MainMenu != nullptr))
        return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenHostMenu()
{
    if (!ensure(MenuSwitcher != nullptr))
        return;

    if (!ensure(HostMenu != nullptr))
        return;

    MenuSwitcher->SetActiveWidget(HostMenu);
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

void UMainMenu::UpdateServerListChildren()
{
    for (size_t i = 0; i < ServerList->GetChildrenCount(); i++)
    {
        UServerRow *ServerRow = Cast<UServerRow>(ServerList->GetChildAt(i));
        if (!ServerRow)
        {
            continue;
        }

        ServerRow->Selected = SelectedIndex.IsSet() && SelectedIndex.GetValue() == i;
    }
}
