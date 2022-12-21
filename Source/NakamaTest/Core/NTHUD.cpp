// Fill out your copyright notice in the Description page of Project Settings.


#include "NTHUD.h"
#include "Blueprint/UserWidget.h"
#include "NakamaTest/Widget/NTChatWidget.h"
#include "NakamaTest/Widget/NTLoginWidget.h"

void ANTHUD::BeginPlay()
{
	Super::BeginPlay();


	if (ChatWidgetClass)
	{
		ChatWidget = CreateWidget<UNTChatWidget>(GetOwningPlayerController(), ChatWidgetClass);
	}
	
	if (LoginWidgetClass)
	{
		LoginWidget = CreateWidget<UNTLoginWidget>(GetOwningPlayerController(), LoginWidgetClass);
		LoginWidget->OnAuthenticationCompleted().AddLambda([this]()
		{
			LoginWidget->RemoveFromParent();
			ChatWidget->AddToViewport();
		});
		LoginWidget->AddToViewport();
	}

	if (auto *controller = GetOwningPlayerController())
	{
		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeUIOnly());
	}
}
