// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"
#include "EOSGameSession.h"
#include "GameFramework/GameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "MainMenuPlayerController.h"
#include "EOSSessionSubsystem.h"

void UEOSGameInstance::Init()
{
	Super::Init();

	UEOSSessionSubsystem* SessionSubsystem = GetSubsystem<UEOSSessionSubsystem>();
	if (ensure(SessionSubsystem))
	{
		SessionSubsystem->OnJoinSessionsCompletedEvent.AddUObject(this, &UEOSGameInstance::OnJoinSessionsComplete);
		SessionSubsystem->OnFindSessionsCompletedEvent.AddUObject(this, &UEOSGameInstance::OnFindSessionsComplete);
	}
}

AMainMenuPlayerController* UEOSGameInstance::GetMainMenuPlayerController() const
{
	if (GetFirstLocalPlayerController() == NULL)
		return NULL;
	return Cast<AMainMenuPlayerController>(GetFirstLocalPlayerController());
}

bool UEOSGameInstance::JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult)
{
	UEOSSessionSubsystem* SessionSubsystem = GetSubsystem<UEOSSessionSubsystem>();
	if (ensure(SessionSubsystem))
	{
		return SessionSubsystem->JoinSession(SearchResult);
	}

	return false;
}

void UEOSGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (GetMainMenuPlayerController())
		GetMainMenuPlayerController()->OnJoinSessionCompleted(SessionName, Result);
}

void UEOSGameInstance::FindSession(FName SearchKey, FString SearchValue)
{
	UEOSSessionSubsystem* SessionSubsystem = GetSubsystem<UEOSSessionSubsystem>();
	if (ensure(SessionSubsystem))
	{
		SessionSubsystem->FindSession(SearchKey, SearchValue);
	}
}

void UEOSGameInstance::OnFindSessionsComplete(bool bWasSuccessful) const
{
	if (GetMainMenuPlayerController())
		GetMainMenuPlayerController()->OnFindSessionCompleted(bWasSuccessful);
}