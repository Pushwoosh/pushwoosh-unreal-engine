// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class PushwooshImpl;
class PushwooshSettings;

DECLARE_MULTICAST_DELEGATE_OneParam(FPushwooshRegistrationSuccessDelegate, FString);
DECLARE_MULTICAST_DELEGATE_OneParam(FPushwooshRegistrationErrorDelegate, FString);
DECLARE_MULTICAST_DELEGATE_OneParam(FPushwooshPushAcceptedDelegate, FString);

class FPushwooshModule : public IModuleInterface
{
public:

	static inline FPushwooshModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FPushwooshModule >( "Pushwoosh" );
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "Pushwoosh" );
	}
	
	/**
	 * Push registration success event. Is triggered after successful RegisterForPushNotifications() call.
	 *
	 * Parameters:
	 *   FString pushToken - push notification token.
	 */
	static FPushwooshRegistrationSuccessDelegate PushRegistrationSucceeded;
	
	/**
	 * Push registration error event. Is triggered if RegisterForPushNotifications() method fails.
	 *
	 * Parameters:
	 *   FString error - error description.
	 */
	static FPushwooshRegistrationErrorDelegate PushRegistrationError;
	
	/**
	 * Push notification accept event. Is triggered when user taps on push notification.
	 *
	 * Parameters:
	 *   FString json - push notification payload in JSON format.
	 */
	static FPushwooshPushAcceptedDelegate PushAccepted;

	/**
	 * Initializes plugin with specified Settings (Edit -> Project Settings... -> Plugins -> Pushwoosh).
	 * Tells plugin that client is ready to receive PushRegistrationSucceeded, PushRegistrationError and PushAccepted Delegate events.
	 * Sends application open statistics.
	 * Should be called on every application start before invoking any other module methods.
	 */
	void Initialize();

	/**
	 * Request push notification registration. This will result in notification permisstion dialog on iOS.
	 * If registration succeeds PushRegistrationSucceeded is broadcasted.
	 * If registration fails PushRegistrationError is broadcasted.
	 * 
	 * Note: On iOS it is recommened to set 'UIBackgroundModes: remote-notification' in Into.plist 
	 * 		 to be able to get push token and register for push notifications regardless of user choice.
	 * 		 Also if 'UIBackgroundModes: remote-notification' is not set and user disables push notifications no callback will be invoked.
	 */
	void RegisterForPushNotifications();

	/**
	 * Unregisters device from push notifications and tells server that device is no longer subscribed.
	 */
	void UnregisterForPushNotifications();

	/**
	 * Associates device with given (tagName, tagValue) pair.
	 */
	void SetIntTag(FString tagName, int tagValue);

	/** 
	 * Associates device with given (tagName, tagValue) pair.
	 */
	void SetStringTag(FString tagName, FString tagValue);

	/** 
	 * Associates device with given tags in JSON format. Multiple tags can be specified in a single request.
	 *
	 * Example:
	 *   FPushwooshModule& pushwoosh = FPushwooshModule::Get();
	 *	 pushwoosh.SetTags("{ \"intTag\" : 1, \"stringTag\" : \"2\", \"listTag\" : [ \"3\", \"4\", \"5\" ] }");
	 */
	void SetTags(FString json);

	/**
	 * Set User indentifier. This could be Facebook ID, username or email, or any other user ID.
 	 * This allows data and events to be matched across multiple user devices.
	 */
	void SetUserId(FString userId);

	/**
	 * Post events for In-App Messages. This can trigger In-App message display as specified in Pushwoosh Control Panel.
	 * @param event name of the event
	 * @param attributes JSON object with additional event parameters
	 * 
	 * Example:
	 *   FPushwooshModule& pushwoosh = FPushwooshModule::Get();
	 *	 pushwoosh.PostEvent("buttonPressed", "{ \"buttonNumber\" : \"4\", \"buttonLabel\" : \"Banner\" }");
	 */
	void PostEvent(FString event, FString attributes = "{}");

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	PushwooshImpl *pImpl;
};
