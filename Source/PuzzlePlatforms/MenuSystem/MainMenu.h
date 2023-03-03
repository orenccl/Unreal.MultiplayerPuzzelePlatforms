// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;
class IMenuInterface;

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UWidget *MainMenu;

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
	UButton *CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton *ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox *IPAddressField;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void QuitGame();
};
