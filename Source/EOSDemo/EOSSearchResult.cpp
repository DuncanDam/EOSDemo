// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSSearchResult.h"

FString UEOSSearchResult::GetDescription() const
{
	return Result.GetSessionIdStr();
}

void UEOSSearchResult::GetStringSetting(FName Key, FString& Value, bool& bFoundValue) const
{
	bFoundValue = Result.Session.SessionSettings.Get<FString>(Key, /*out*/ Value);
}

void UEOSSearchResult::GetIntSetting(FName Key, int32& Value, bool& bFoundValue) const
{
	bFoundValue = Result.Session.SessionSettings.Get<int32>(Key, /*out*/ Value);
}

int32 UEOSSearchResult::GetNumOpenPrivateConnections() const
{
	return Result.Session.NumOpenPrivateConnections;
}

int32 UEOSSearchResult::GetNumOpenPublicConnections() const
{
	return Result.Session.NumOpenPublicConnections;
}

int32 UEOSSearchResult::GetMaxPublicConnections() const
{
	return Result.Session.SessionSettings.NumPublicConnections;
}

int32 UEOSSearchResult::GetPingInMs() const
{
	return Result.PingInMs;
}

int32 UEOSSearchResult::GetCurrentPlayers() const
{
	return Result.Session.SessionSettings.NumPublicConnections
		+ Result.Session.SessionSettings.NumPrivateConnections
		- Result.Session.NumOpenPublicConnections
		- Result.Session.NumOpenPrivateConnections;
}

FString UEOSSearchResult::GetMapName() const
{
	FString Map;
	Result.Session.SessionSettings.Get(SETTING_MAPNAME, Map);
	return Map;
}

FString UEOSSearchResult::GetGameMode() const
{
	FString GameMode;
	Result.Session.SessionSettings.Get(SETTING_GAMEMODE, GameMode);
	return GameMode;
}