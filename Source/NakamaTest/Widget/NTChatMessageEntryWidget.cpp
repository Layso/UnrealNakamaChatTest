// Fill out your copyright notice in the Description page of Project Settings.


#include "NTChatMessageEntryWidget.h"

#include "JsonObjectConverter.h"
#include "Components/TextBlock.h"
#include "NakamaTest/Subsystem/NTNakamaSubsystem.h"

void UNTChatMessageEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (auto *data = Cast<UNTChatMessageEntryData>(ListItemObject))
	{
		FMessageData messageData;
		FJsonObjectConverter::JsonObjectStringToUStruct(data->MessageData.Content, &messageData);
		
		TimestampText->SetText(FText::FromString(data->MessageData.CreateTime.ToString(TEXT("%H:%M"))));
		UsernameText->SetText(FText::FromString(data->MessageData.Username));
		MessageText->SetText(FText::FromString(messageData.Message));
	}
}
