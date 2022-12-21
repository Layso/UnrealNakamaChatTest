// Fill out your copyright notice in the Description page of Project Settings.


#include "NTChatWidget.h"

#include "NTChatMessageEntryWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "NakamaTest/Subsystem/NTNakamaSubsystem.h"

FBackRequested& UNTChatWidget::OnLogOutRequested()
{
	return BackRequested;
}

void UNTChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UNTChatWidget::OnBackButtonClicked);
	SendButton->OnClicked.AddDynamic(this, &UNTChatWidget::OnSendButtonClicked);
	if (auto *nakama = GetGameInstance()->GetSubsystem<UNTNakamaSubsystem>())
	{
		UsernameText->SetText(FText::FromString(nakama->GetUsername()));
		
		nakama->OnChatMessageReceived().BindLambda([this](const FNakamaChannelMessage& Message)
		{
			if (auto *data = NewObject<UNTChatMessageEntryData>())
			{
				data->MessageData = Message;
				MessageList->AddItem(data);
			}
		});

		nakama->JoinChatRoom(TEXT("GlobalChat"), FChatJoinResulted::CreateLambda([this](bool bIsSuccess, ENakamaRtErrorCode ErrorCode)
		{
			FText errorMessage;
			Switcher->SetActiveWidgetIndex(bIsSuccess ? 1 : 0);

			if (!bIsSuccess)
			{
				if (auto *label = ErrorLabels.Find(ErrorCode))
				{
					errorMessage = *label;
				}

				else
				{
					errorMessage = UnknownErrorLabel;
				}
			}
			
			ErrorMessageText->SetText(errorMessage);
		}));
	}
}

void UNTChatWidget::OnBackButtonClicked()
{
	if (auto *nakama = GetGameInstance()->GetSubsystem<UNTNakamaSubsystem>())
	{
		nakama->Logout(FAuthenticationResulted::CreateLambda([this](bool bIsSuccess, ENakamaErrorCode ErrorCode)
		{
			if (BackRequested.IsBound())
			{
				BackRequested.Broadcast();
			}
		}));
	}
}

void UNTChatWidget::OnSendButtonClicked()
{
	if (!MessageField->GetText().IsEmpty())
	{
		if (auto *nakama = GetGameInstance()->GetSubsystem<UNTNakamaSubsystem>())
		{
			SendButton->SetIsEnabled(false);
			nakama->SendMessage(MessageField->GetText().ToString(), FChatSendResulted::CreateLambda([this](bool bIsSuccess)
			{
				if (bIsSuccess)
				{
					MessageField->SetText(FText());
				}
				
				SendButton->SetIsEnabled(true);
			}));
		}
	}
}
