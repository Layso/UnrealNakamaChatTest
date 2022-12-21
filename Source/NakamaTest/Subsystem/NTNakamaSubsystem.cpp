// Fill out your copyright notice in the Description page of Project Settings.


#include "NTNakamaSubsystem.h"

#include "JsonObjectConverter.h"
#include "NakamaClient.h"
#include "Interfaces/OnlineChatInterface.h"

void UNTNakamaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FString serverKey = TEXT("defaultkey");
	FString host = TEXT("localhost");
	int32 port = 7350;

	Client = UNakamaClient::CreateDefaultClient(serverKey, host, port);
}

void UNTNakamaSubsystem::Logout(const FAuthenticationResulted& ResultDelegate)
{
	Session = nullptr;
	if (ResultDelegate.IsBound())
	{
		ResultDelegate.Execute(true, ENakamaErrorCode::Unknown);
	}
}

void UNTNakamaSubsystem::Login(const FString& Email, const FString& Password, const FAuthenticationResulted& ResultDelegate)
{
	FOnError failDelegate;
	FOnAuthUpdate successDelegate;

	failDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnAuthenticationFail);
	successDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnAuthenticationSuccess);

	AuthenticationResulted = ResultDelegate;
	
	Client->AuthenticateEmail(Email, Password, FString(), false, TMap<FString, FString>(), successDelegate, failDelegate);
}

void UNTNakamaSubsystem::Register(const FString& Email, const FString& Password, const FString& Username, const FAuthenticationResulted& ResultDelegate)
{
	FOnError failDelegate;
	FOnAuthUpdate successDelegate;

	failDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnAuthenticationFail);
	successDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnAuthenticationSuccess);

	AuthenticationResulted = ResultDelegate;
	
	Client->AuthenticateEmail(Email, Password, Username, true, TMap<FString, FString>(), successDelegate, failDelegate);
}

const FString& UNTNakamaSubsystem::GetUsername()
{
	return Session->SessionData.Username;
}

void UNTNakamaSubsystem::JoinChatRoom(const FString& RoomName, const FChatJoinResulted& ResultDelegate)
{
	if (Socket)
	{
		FOnRtError failDelegate;
		FOnJoinChat successDelegate;

		failDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnChatJoinFail);
		successDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnChatJoinSuccess);

		ChatJoinResulted = ResultDelegate;
		
		Socket->JoinChat(RoomName, ENakamaChannelType::ROOM, true, false, successDelegate, failDelegate);
	}
}

const FNakamaChannel& UNTNakamaSubsystem::GetChatChannel()
{
	return ChatChannel;
}

void UNTNakamaSubsystem::SendMessage(const FString& Message, const FChatSendResulted& ResultDelegate)
{
	if (Socket)
	{
		ChatSendResulted = ResultDelegate;
		
		FOnRtError failDelegate;
		FOnWriteChatMessage successDelegate;

		failDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnChatMessageFail);
		successDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnChatMessageSuccess);

		
		FMessageData messageData;
		messageData.Message = Message;

		FString messageJson;
		FJsonObjectConverter::UStructToJsonObjectString(messageData, messageJson);
		
		Socket->SendMessage(ChatChannel.Id, messageJson, successDelegate, failDelegate);
	}
}

FChatMessageReceived& UNTNakamaSubsystem::OnChatMessageReceived()
{
	return ChatMessageReceived;
}

void UNTNakamaSubsystem::OnAuthenticationFail(const FNakamaError& ErrorData)
{
	if (AuthenticationResulted.IsBound())
	{
		AuthenticationResulted.Execute(false, ErrorData.Code);
	}
}

void UNTNakamaSubsystem::OnAuthenticationSuccess(UNakamaSession* LoginData)
{
	Session = LoginData;
	Socket = Client->SetupRealtimeClient(Session, true);
	
	FOnRealtimeClientError failDelegate;
	FOnRealtimeClientConnected successDelegate;
	
	failDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnSocketConnectionFail);
	successDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnSocketConnectionSuccess);

	Socket->Connect(successDelegate, failDelegate);
}

void UNTNakamaSubsystem::OnSocketConnectionFail()
{
	if (AuthenticationResulted.IsBound())
	{
		AuthenticationResulted.Execute(false, ENakamaErrorCode::Unknown);
	}
}

void UNTNakamaSubsystem::OnSocketConnectionSuccess()
{
	if (AuthenticationResulted.IsBound())
	{
		AuthenticationResulted.Execute(true, ENakamaErrorCode::Unknown);
	}
}

void UNTNakamaSubsystem::OnChatJoinFail(const FNakamaRtError& ErrorData)
{
	if (ChatJoinResulted.IsBound())
	{
		ChatJoinResulted.Execute(false, ErrorData.Code);
	}
}

void UNTNakamaSubsystem::OnChatJoinSuccess(FNakamaChannel ChannelData)
{
	ChatChannel = ChannelData;
	
	FOnReceivedChannelMessage messageReceivedDelegate;
	messageReceivedDelegate.AddDynamic(this, &UNTNakamaSubsystem::OnChatMessageReceived);
	Socket->ChannelMessageReceived = messageReceivedDelegate;
	Socket->SetListenerChannelMessageCallback();
	
	if (ChatJoinResulted.IsBound())
	{
		ChatJoinResulted.Execute(true, ENakamaRtErrorCode::UNKNOWN);
	}
}

void UNTNakamaSubsystem::OnChatMessageSuccess(FNakamaChannelMessageAck MessageData)
{
	if (ChatSendResulted.IsBound())
	{
		ChatSendResulted.Execute(true);
	}
}

void UNTNakamaSubsystem::OnChatMessageFail(const FNakamaRtError& ErrorData)
{
	if (ChatSendResulted.IsBound())
	{
		ChatSendResulted.Execute(false);
	}
}

void UNTNakamaSubsystem::OnChatMessageReceived(const FNakamaChannelMessage& Message)
{
	if (ChatMessageReceived.IsBound())
	{
		ChatMessageReceived.Execute(Message);
	}
}