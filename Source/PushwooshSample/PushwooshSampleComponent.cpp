// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#include "PushwooshSample.h"
#include "PushwooshSampleComponent.h"

#include "Pushwoosh.h"

// Sets default values for this component's properties
UPushwooshSampleComponent::UPushwooshSampleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	// ...
}


// Called when the game starts
void UPushwooshSampleComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Pushwoosh
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule::PushRegistrationSucceeded.AddUObject(this, &UPushwooshSampleComponent::PushRegistrationSucceeded_Handler);
		FPushwooshModule::PushRegistrationError.AddUObject(this, &UPushwooshSampleComponent::PushRegistrationError_Handler);
		FPushwooshModule::PushAccepted.AddUObject(this, &UPushwooshSampleComponent::PushAccepted_Handler);
		
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.Initialize();
		pushwoosh.RegisterForPushNotifications();
	}
	
}

void UPushwooshSampleComponent::PushRegistrationSucceeded_Handler(FString token)
{
	UE_LOG(LogPushwooshSample, Log, TEXT("Pushwoosh component registered for pushes with token: %s"), *token);
}

void UPushwooshSampleComponent::PushRegistrationError_Handler(FString error)
{
	UE_LOG(LogPushwooshSample, Log, TEXT("Pushwoosh component failed to register for pushes: %s"), *error);
}

void UPushwooshSampleComponent::PushAccepted_Handler(FString data)
{
	UE_LOG(LogPushwooshSample, Log, TEXT("Pushwoosh component push notification opened %s"), *data);
}
