// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "EOSGameInstance.h"
#include "EOSGameSession.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{

}

// Copy from EOSPlayerController.cpp
// Can move this to 1 base Player controller class instead
void AMainMenuPlayerController::Login()
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();

    FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);

    if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
    {
        return;
    }

    LoginDelegateHandle =
        Identity->AddOnLoginCompleteDelegate_Handle(
            0,
            FOnLoginCompleteDelegate::CreateUObject(
                this,
                &ThisClass::HandleLoginCompleted));
    
    FString AuthType;
    FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

    if (!AuthType.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

        if (!Identity->AutoLogin(0))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... "));
            Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();
        }
    }
    else
    {
        FOnlineAccountCredentials Credentials("AccountPortal", "", "");

        UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

        if (!Identity->Login(0, Credentials))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... "));

            // Clear our handle and reset the delegate. 
            Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();
        }
    }
}

// Copy from EOSPlayerController.cpp
// Can move this to base Player controller class instead
void AMainMenuPlayerController::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));
        UE_LOG(LogTemp, Log, TEXT("Loading cloud data and searching for a session..."));

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EOS login failed."));
    }

    K2_OnUserLoginCompleted_Dynamic.Broadcast(bWasSuccessful);

    // Clear our handle and reset the delegate. 
    Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
    LoginDelegateHandle.Reset();
}

void AMainMenuPlayerController::FindSession(FName SearchKey, FString SearchValue)
{
    ULocalPlayer* PlayerOwner = Cast<ULocalPlayer>(this->Player);
    UEOSGameInstance* GI = Cast<UEOSGameInstance>(this->GetGameInstance());
    if (GI)
        GI->FindSession(SearchKey, SearchValue);
}

void AMainMenuPlayerController::OnFindSessionCompleted(bool bSuccessful)
{
    K2_OnFindSessionCompleted_Dynamic.Broadcast(bSuccessful);
}

void AMainMenuPlayerController::JoinSession(const UEOSSearchResult* Search)
{
    ULocalPlayer* PlayerOwner = Cast<ULocalPlayer>(this->Player);
    UEOSGameInstance* GI = Cast<UEOSGameInstance>(this->GetGameInstance());
    if (GI)
        GI->JoinSession(PlayerOwner, Search->Result);
}

void AMainMenuPlayerController::OnJoinSessionCompleted(FName Session, EOnJoinSessionCompleteResult::Type Result)
{
    K2_OnJoinSessionCompleted_Dynamic.Broadcast(Result == EOnJoinSessionCompleteResult::Success);
}
