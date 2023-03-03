// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class IMenuInterface;

/**
 * Base Menu Widget
 */

UCLASS()
class PUZZLEPLATFORMS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

	void SetMenuInterface(IMenuInterface *Interface);

protected:
	/** Automaticaly call when widget destruct. */
	virtual void NativeDestruct() override;
	/** Instance implement menu system */
	IMenuInterface *MenuInterface;
};
