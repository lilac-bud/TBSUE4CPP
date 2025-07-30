// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UCanvasPanel;

UCLASS()
class TBSUE4CPP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainMenu;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* Settings;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* PlayButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SettingsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ExitButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SettingsBackToMenuButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USpinBox* IterationsAmount;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UComboBoxString* PlayersSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SaveSettingsButton;

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void SettingsButtonClicked();
	UFUNCTION()
	void SettingsBackToMenuButtonClicked();
	UFUNCTION()
	void SaveSettingsButtonClicked();
};
