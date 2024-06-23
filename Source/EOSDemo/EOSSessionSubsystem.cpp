// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"

void UEOSSessionSubsystem::FindSession(FName SearchKey, FString SearchValue)
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

    Search->QuerySettings.SearchParams.Empty();
    Search->QuerySettings.Set(SearchKey, SearchValue, EOnlineComparisonOp::Equals);

    SearchResults.Empty();

    FindSessionsDelegateHandle =
        Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleFindSessionsCompleted,
            Search));

    if (!Session->FindSessions(0, Search))
    {
        UE_LOG(LogTemp, Warning, TEXT("Finding session failed."));
        // Clear our handle and reset the delegate. 
        Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
        FindSessionsDelegateHandle.Reset();
        RecentSearch = Search;
    }
}

void UEOSSessionSubsystem::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

    if (Search->SearchResults.Num() > 0)
    {
        for (int32 i = 0; i < Search->SearchResults.Num(); ++i)
        {
            auto& Result = Search->SearchResults[i];
            DumpSession(&Result.Session);

            UEOSSearchResult* Entry = NewObject<UEOSSearchResult>(this);
            Entry->Result = Result;
            SearchResults.Add(Entry);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("HandleFindSessionsCompleted"));

    RecentSearch = Search;

    if (OnFindSessionsCompletedEvent.IsBound())
        OnFindSessionsCompletedEvent.Broadcast(bWasSuccessful);

    // Clear our handle and reset the delegate. 
    Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
    FindSessionsDelegateHandle.Reset();
}

bool UEOSSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult) {
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

    JoinSessionsDelegateHandle =
        Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleJoinSessionsCompleted));

    return Session->JoinSession(0, SessionName, SearchResult);
}

void UEOSSessionSubsystem::HandleJoinSessionsCompleted(FName Session, EOnJoinSessionCompleteResult::Type Result)
{
    UE_LOG(LogTemp, Error, TEXT("HandleJoinSessionsCompleted!"));
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("InternalClientTravel!"));
        InternalClientTravel(Session);
    }

    if (OnJoinSessionsCompletedEvent.IsBound())
        OnJoinSessionsCompletedEvent.Broadcast(Session, Result);
}

void UEOSSessionSubsystem::InternalClientTravel(FName Session)
{
    APlayerController* const PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
    if (PlayerController == nullptr)
    {
        FText ReturnReason = NSLOCTEXT("NetworkErrors", "InvalidPlayerController", "Invalid Player Controller");
        UE_LOG(LogTemp, Error, TEXT("InternalTravelToSession(Failed due to %s)"), *ReturnReason.ToString());
        return;
    }

    FString URL;
    // travel to session
    IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
    check(OnlineSub);

    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
    check(Sessions.IsValid());

    UE_LOG(LogTemp, Error, TEXT("Start to travel"));


    if (!Sessions->GetResolvedConnectString(SessionName, URL))
    {
        FText FailReason = NSLOCTEXT("NetworkErrors", "TravelSessionFailed", "Travel to Session failed.");
        UE_LOG(LogTemp, Error, TEXT("InternalTravelToSession(%s)"), *FailReason.ToString());
        return;
    }
    // Change the condition if testing editor with deloyed server.
#if !UE_BUILD_SHIPPING
    // Travel to localhost for local test since we are one same IP also to prevent issue like NAT, firewall and as such
    PlayerController->ClientTravel("127.0.0.1", TRAVEL_Absolute);
#else
    PlayerController->ClientTravel(URL, TRAVEL_Absolute);
#endif

}