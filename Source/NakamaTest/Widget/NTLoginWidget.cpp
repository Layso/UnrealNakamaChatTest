// Fill out your copyright notice in the Description page of Project Settings.


#include "NTLoginWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "NakamaTest/Subsystem/NTNakamaSubsystem.h"

FAuthenticationCompleted& UNTLoginWidget::OnAuthenticationCompleted()
{
	return AuthenticationCompleted;
}

void UNTLoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoginButton->OnClicked.AddDynamic(this, &UNTLoginWidget::OnLoginClicked);
	RegisterButton->OnClicked.AddDynamic(this, &UNTLoginWidget::OnRegisterClicked);
	LoginTabButton->OnClicked.AddDynamic(this, &UNTLoginWidget::OnLoginTabClicked);
	RegisterTabButton->OnClicked.AddDynamic(this, &UNTLoginWidget::OnRegisterTabClicked);
}

void UNTLoginWidget::UpdateButtonStatus(bool bNewEnabled)
{
	LoginButton->SetIsEnabled(bNewEnabled);
	RegisterButton->SetIsEnabled(bNewEnabled);
	LoginTabButton->SetIsEnabled(bNewEnabled);
	RegisterTabButton->SetIsEnabled(bNewEnabled);
}

void UNTLoginWidget::OnLoginTabClicked()
{
	MenuSwitcher->SetActiveWidget(LoginTabContainer);
}

void UNTLoginWidget::OnRegisterTabClicked()
{
	MenuSwitcher->SetActiveWidget(RegisterTabContainer);
}

void UNTLoginWidget::OnLoginClicked()
{
	FString mail = LoginMailField->GetText().ToString();
	FString pass = LoginPassField->GetText().ToString();
	
	UpdateButtonStatus(false);
	ErrorText->SetText(FText());
	
	if (auto *nakama = GetGameInstance()->GetSubsystem<UNTNakamaSubsystem>())
	{
		nakama->Login(mail, pass, FAuthenticationResulted::CreateUObject(this, &UNTLoginWidget::OnAuthenticationResulted));
	}
}

void UNTLoginWidget::OnRegisterClicked()
{
	FString mail = RegisterMailField->GetText().ToString();
	FString pass = RegisterPassField->GetText().ToString();
	FString nick = RegisterNickField->GetText().ToString();
	
	UpdateButtonStatus(false);
	ErrorText->SetText(FText());

	if (auto *nakama = GetGameInstance()->GetSubsystem<UNTNakamaSubsystem>())
	{
		nakama->Register(mail, pass, nick, FAuthenticationResulted::CreateUObject(this, &UNTLoginWidget::OnAuthenticationResulted));
	}
}

void UNTLoginWidget::OnAuthenticationResulted(bool bIsSuccess, ENakamaErrorCode ErrorCode)
{
	if (bIsSuccess)
	{
		if (AuthenticationCompleted.IsBound())
		{
			AuthenticationCompleted.Broadcast();
		}
	}

	else if (auto *label = ErrorLabels.Find(ErrorCode))
	{
		ErrorText->SetText(*label);
		UpdateButtonStatus(true);
	}

	else
	{
		ErrorText->SetText(UnknownErrorLabel);
		UpdateButtonStatus(true);
	}
}
