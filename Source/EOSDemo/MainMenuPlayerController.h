// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionCompleted_Dynamic, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class EOSDEMO_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainMenuPlayerController();

	UFUNCTION(BlueprintCallable)
	void Login();

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (DisplayName = "On User Login Completed"))
	FOnActionCompleted_Dynamic K2_OnUserLoginCompleted_Dynamic;

	FDelegateHandle LoginDelegateHandle;
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UFUNCTION(BlueprintCallable, Category = Session)
	void FindSession(FName SearchKey, FString SearchValue);

	void OnFindSessionCompleted(bool bSuccessful);

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (DisplayName = "On Find Session Completed"))
	FOnActionCompleted_Dynamic K2_OnFindSessionCompleted_Dynamic;

	UFUNCTION(BlueprintCallable, Category = Session)
	void JoinSession(const UEOSSearchResult* Search);

	void OnJoinSessionCompleted(FName Session, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (DisplayName = "On Join Session Completed"))
	FOnActionCompleted_Dynamic K2_OnJoinSessionCompleted_Dynamic;
};
