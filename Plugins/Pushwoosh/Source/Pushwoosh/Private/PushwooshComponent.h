// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "PushwooshComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushwooshRegistrationSuccessDynamicDelegate, FString, PushToken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushwooshRegistrationErrorDynamicDelegate, FString, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushwooshPushAcceptedDynamicDelegate, FString, Data);

UCLASS( ClassGroup=(Pushwoosh), meta=(BlueprintSpawnableComponent) )
class UPushwooshComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPushwooshComponent();

	UPROPERTY(BlueprintAssignable, Category = "Pushwoosh")
	FPushwooshRegistrationSuccessDynamicDelegate PushRegistrationSucceeded;

	UPROPERTY(BlueprintAssignable, Category = "Pushwoosh")
	FPushwooshRegistrationErrorDynamicDelegate PushRegistrationError;

	UPROPERTY(BlueprintAssignable, Category = "Pushwoosh")
	FPushwooshPushAcceptedDynamicDelegate PushAccepted;


	UFUNCTION(BlueprintCallable, Category="Pushwoosh")
	void RegisterForPushNotifications();

	UFUNCTION(BlueprintCallable, Category="Pushwoosh")
	void UnregisterForPushNotifications();

	UFUNCTION(BlueprintCallable, Category="Pushwoosh")
	void SetIntTag(FString tagName, int tagValue);
	
	UFUNCTION(BlueprintCallable, Category="Pushwoosh")
	void SetStringTag(FString tagName, FString tagValue);
	
	UFUNCTION(BlueprintCallable, Category="Pushwoosh")
	void SetTags(FString json);

	UFUNCTION(BlueprintCallable, Category="Pushwoosh")
	void SetUserId(FString userId);

	UFUNCTION(BlueprintCallable, Category="Pushwoosh")
	void PostEvent(FString event, FString attributes = "");
	
	void InitializeComponent() override;

private:	
	void PushRegistrationSucceeded_Handler(FString token);
	void PushRegistrationError_Handler(FString error);
	void PushAccepted_Handler(FString data);
};
