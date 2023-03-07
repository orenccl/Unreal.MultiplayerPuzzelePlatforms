// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UEditableTextBox;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;
class UPanelWidget;
class IMenuInterface;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	FString HostUserName;

	uint16 CurrentPlayers;
	uint16 MaxPlayers;
};

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer &ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerDatas);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UWidget *MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget *HostMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget *JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher *MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton *HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton *JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton *QuitButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox *ServerName;

	UPROPERTY(meta = (BindWidget))
	UButton *CancelHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton *ConfirmHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton *CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton *ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget *ServerList;

	TSubclassOf<UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void QuitGame();

	/** Update children selected state */
	void UpdateServerListChildren();
};
