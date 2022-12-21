// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NakamaClient.h"
#include "NTNakamaSubsystem.generated.h"

class UNakamaClient;

USTRUCT()
struct FMessageData
{
	GENERATED_BODY()

	UPROPERTY()
	FString Message;

	UPROPERTY()
	FString Emote;

	UPROPERTY()
	FString EmoteTarget;

	UPROPERTY()
	FString MatchId;

	UPROPERTY()
	FString PartyId;
};

DECLARE_DELEGATE_OneParam(FChatSendResulted, bool)
DECLARE_DELEGATE_OneParam(FChatMessageReceived, const FNakamaChannelMessage&)
DECLARE_DELEGATE_TwoParams(FChatJoinResulted, bool, ENakamaRtErrorCode)
DECLARE_DELEGATE_TwoParams(FAuthenticationResulted, bool, ENakamaErrorCode)

/**
 * 
 */
UCLASS()
class NAKAMATEST_API UNTNakamaSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void Logout(const FAuthenticationResulted& ResultDelegate);
	void Login(const FString& Email, const FString& Password, const FAuthenticationResulted& ResultDelegate);
	void Register(const FString& Email, const FString& Password, const FString& Username, const FAuthenticationResulted& ResultDelegate);
	const FString& GetUsername();
	
	void JoinChatRoom(const FString& RoomName, const FChatJoinResulted& ResultDelegate);
	const FNakamaChannel& GetChatChannel();
	void SendMessage(const FString& Message, const FChatSendResulted& ResultDelegate);
	FChatMessageReceived& OnChatMessageReceived();
	
private:
	FNakamaChannel ChatChannel;

	FChatJoinResulted ChatJoinResulted;
	FChatSendResulted ChatSendResulted;
	FChatMessageReceived ChatMessageReceived;
	FAuthenticationResulted AuthenticationResulted;

	UPROPERTY()	UNakamaClient *Client;
	UPROPERTY()	UNakamaSession *Session;
	UPROPERTY()	UNakamaRealtimeClient *Socket;

	UFUNCTION() void OnAuthenticationFail(const FNakamaError& ErrorData);
	UFUNCTION()	void OnAuthenticationSuccess(UNakamaSession *LoginData);

	UFUNCTION() void OnSocketConnectionFail();
	UFUNCTION() void OnSocketConnectionSuccess();

	UFUNCTION() void OnChatJoinFail(const FNakamaRtError& ErrorData);
	UFUNCTION() void OnChatJoinSuccess(FNakamaChannel ChannelData);

	UFUNCTION() void OnChatMessageFail(const FNakamaRtError& ErrorData);
	UFUNCTION() void OnChatMessageSuccess(FNakamaChannelMessageAck MessageData);
	UFUNCTION() void OnChatMessageReceived(const FNakamaChannelMessage& Message);
};
