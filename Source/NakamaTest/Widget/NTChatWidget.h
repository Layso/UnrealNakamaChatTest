// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NakamaRtError.h"
#include "Blueprint/UserWidget.h"
#include "NTChatWidget.generated.h"

class UButton;
class UListView;
class UTextBlock;
class UWidgetSwitcher;
class UEditableTextBox;

DECLARE_MULTICAST_DELEGATE(FBackRequested)

/**
 * 
 */
UCLASS()
class NAKAMATEST_API UNTChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FBackRequested& OnLogOutRequested();
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	FText UnknownErrorLabel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	TMap<ENakamaRtErrorCode, FText> ErrorLabels;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton *BackButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton *SendButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock *UsernameText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock *ErrorMessageText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox *MessageField;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher *Switcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UListView *MessageList;

private:
	FBackRequested BackRequested;

	UFUNCTION() void OnBackButtonClicked();
	UFUNCTION() void OnSendButtonClicked();
};
