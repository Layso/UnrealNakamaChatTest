// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NTHUD.generated.h"

class UNTChatWidget;
class UNTLoginWidget;

UCLASS()
class NAKAMATEST_API ANTHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Setup")
	TSubclassOf<UNTChatWidget> ChatWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="Setup")
	TSubclassOf<UNTLoginWidget> LoginWidgetClass;

private:
	UPROPERTY()
	UNTChatWidget *ChatWidget;
	
	UPROPERTY()
	UNTLoginWidget *LoginWidget;
};
