// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NakamaChannelTypes.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "NTChatMessageEntryWidget.generated.h"

class UTextBlock;

UCLASS()
class NAKAMATEST_API UNTChatMessageEntryData : public UObject
{
	GENERATED_BODY()
	
public:
	FNakamaChannelMessage MessageData;
};

/**
 * 
 */
UCLASS()
class NAKAMATEST_API UNTChatMessageEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock *UsernameText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock *MessageText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock *TimestampText;
};
