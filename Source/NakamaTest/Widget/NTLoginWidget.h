// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NakamaError.h"
#include "Blueprint/UserWidget.h"
#include "NTLoginWidget.generated.h"

class UButton;
class UOverlay;
class UTextBlock;
class UWidgetSwitcher;
class UEditableTextBox;

DECLARE_MULTICAST_DELEGATE(FAuthenticationCompleted)

/**
 * 
 */
UCLASS()
class NAKAMATEST_API UNTLoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FAuthenticationCompleted& OnAuthenticationCompleted();
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	FText UnknownErrorLabel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	TMap<ENakamaErrorCode, FText> ErrorLabels;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher *MenuSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay *LoginTabContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay *RegisterTabContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton *LoginTabButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton *RegisterTabButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton *LoginButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox *LoginMailField;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox *LoginPassField;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton *RegisterButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox *RegisterMailField;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox *RegisterPassField;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox *RegisterNickField;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock *ErrorText;

private:
	FAuthenticationCompleted AuthenticationCompleted;

	void UpdateButtonStatus(bool bNewEnabled);
	
	UFUNCTION()	void OnLoginTabClicked();
	UFUNCTION()	void OnRegisterTabClicked();
	UFUNCTION()	void OnLoginClicked();
	UFUNCTION()	void OnRegisterClicked();
	UFUNCTION()	void OnAuthenticationResulted(bool bIsSuccess, ENakamaErrorCode ErrorCode);
};
