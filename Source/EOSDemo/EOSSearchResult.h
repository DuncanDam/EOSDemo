// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineSessionSettings.h"
#include "EOSSearchResult.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EOSDEMO_API UEOSSearchResult : public UObject
{
	GENERATED_BODY()
public:
	/** Returns an internal description of the session, not meant to be human readable */
	UFUNCTION(BlueprintCallable, Category = Session)
	FString GetDescription() const;

	/** Gets an arbitrary string setting, bFoundValue will be false if the setting does not exist */
	UFUNCTION(BlueprintPure, Category = Sessions)
	void GetStringSetting(FName Key, FString& Value, bool& bFoundValue) const;

	/** Gets an arbitrary integer setting, bFoundValue will be false if the setting does not exist */
	UFUNCTION(BlueprintPure, Category = Sessions)
	void GetIntSetting(FName Key, int32& Value, bool& bFoundValue) const;

	/** The number of private connections that are available */
	UFUNCTION(BlueprintPure, Category = Sessions)
	int32 GetNumOpenPrivateConnections() const;

	/** The number of publicly available connections that are available */
	UFUNCTION(BlueprintPure, Category = Sessions)
	int32 GetNumOpenPublicConnections() const;

	/** The maximum number of publicly available connections that could be available, including already filled connections */
	UFUNCTION(BlueprintPure, Category = Sessions)
	int32 GetMaxPublicConnections() const;

	/** Ping to the search result, MAX_QUERY_PING is unreachable */
	UFUNCTION(BlueprintPure, Category = Sessions)
	int32 GetPingInMs() const;

	UFUNCTION(BlueprintPure, Category = Sessions)
	int32 GetCurrentPlayers() const;

	UFUNCTION(BlueprintCallable, Category = Session)
	FString GetMapName() const;

	UFUNCTION(BlueprintCallable, Category = Session)
	FString GetGameMode() const;

public:
	FOnlineSessionSearchResult Result;
};
