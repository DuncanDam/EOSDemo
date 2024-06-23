// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"
#include "EOSSearchResult.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOSSessionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class EOSDEMO_API UEOSSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FName SessionName = "SessionName";

public:
	//Addtional flow to handle find session and join session by demand
	TSharedPtr<FOnlineSessionSearch> RecentSearch;

	UPROPERTY(BlueprintReadOnly, Category = Session)
	TArray<TObjectPtr<UEOSSearchResult>> SearchResults;


	void FindSession(FName SearchKey, FString SearchValue);

	FDelegateHandle FindSessionsDelegateHandle;

	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindSessionCompleted, bool);
	FOnFindSessionCompleted OnFindSessionsCompletedEvent;

	bool JoinSession(const FOnlineSessionSearchResult& SearchResult);

	FDelegateHandle JoinSessionsDelegateHandle;

	void HandleJoinSessionsCompleted(FName Session, EOnJoinSessionCompleteResult::Type Result);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnJoinSessionCompleted, FName, EOnJoinSessionCompleteResult::Type);
	FOnJoinSessionCompleted OnJoinSessionsCompletedEvent;

	void InternalClientTravel(FName Session);
};
