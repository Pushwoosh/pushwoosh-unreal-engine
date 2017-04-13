// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#include "PushwooshPrivatePCH.h"

#include "PushwooshComponent.h"

UPushwooshComponent::UPushwooshComponent()
{
	bWantsInitializeComponent = true;
}

void UPushwooshComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule::PushRegistrationSucceeded.AddUObject(this, &UPushwooshComponent::PushRegistrationSucceeded_Handler);
		FPushwooshModule::PushRegistrationError.AddUObject(this, &UPushwooshComponent::PushRegistrationError_Handler);
		FPushwooshModule::PushAccepted.AddUObject(this, &UPushwooshComponent::PushAccepted_Handler);
		
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.Initialize();
	}
	else
	{
		UE_LOG(LogPushwoosh, Error, TEXT("Pushwoosh module unavailable"));
	}
}

void UPushwooshComponent::RegisterForPushNotifications()
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.RegisterForPushNotifications();
	}
}

void UPushwooshComponent::UnregisterForPushNotifications()
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.UnregisterForPushNotifications();
	}
}

void UPushwooshComponent::SetIntTag(FString tagName, int tagValue)
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.SetIntTag(tagName, tagValue);
	}
}


void UPushwooshComponent::SetStringTag(FString tagName, FString tagValue)
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.SetStringTag(tagName, tagValue);
	}
}

void UPushwooshComponent::SetTags(FString json)
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.SetTags(json);
	}
}

void UPushwooshComponent::SetUserId(FString userId)
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.SetUserId(userId);
	}
}

void UPushwooshComponent::PostEvent(FString event, FString attributes)
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.PostEvent(event, attributes);
	}
}

void UPushwooshComponent::PushRegistrationSucceeded_Handler(FString token)
{
	PushRegistrationSucceeded.Broadcast(token);
}

void UPushwooshComponent::PushRegistrationError_Handler(FString error)
{
	PushRegistrationError.Broadcast(error);
}

void UPushwooshComponent::PushAccepted_Handler(FString data)
{
	PushAccepted.Broadcast(data);
}
