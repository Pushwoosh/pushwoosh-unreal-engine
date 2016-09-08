// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#include "PushwooshPrivatePCH.h"

#include "PushwooshImpl.h"

PushwooshImpl::PushwooshImpl()
:	isInitialized(false)
{

}

void PushwooshImpl::Initialize()
{
	isInitialized = true;
}

void PushwooshImpl::RegisterForPushNotifications()
{
	UE_LOG(LogPushwoosh, Warning, TEXT("RegisterForPushNotifications() is not supported on this platform"));
}

void PushwooshImpl::UnregisterForPushNotifications()
{
	UE_LOG(LogPushwoosh, Warning, TEXT("UnregisterForPushNotifications() is not supported on this platform"));
}

void PushwooshImpl::SetIntTag(FString tagName, int tagValue)
{
	UE_LOG(LogPushwoosh, Warning, TEXT("SetIntTag() is not supported on this platform"));
}

void PushwooshImpl::SetStringTag(FString tagName, FString tagValue)
{
	UE_LOG(LogPushwoosh, Warning, TEXT("SetStringTag() is not supported on this platform"));
}

void PushwooshImpl::SetTags(FString json)
{
	UE_LOG(LogPushwoosh, Warning, TEXT("SetTags() is not supported on this platform"));
}
