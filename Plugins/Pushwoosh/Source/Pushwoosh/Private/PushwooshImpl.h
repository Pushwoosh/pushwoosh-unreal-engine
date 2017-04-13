// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

class PushwooshImpl
{
public:
	PushwooshImpl();
	
	virtual void Initialize();

	virtual void RegisterForPushNotifications();

	virtual void UnregisterForPushNotifications();
	
	virtual void SetIntTag(FString tagName, int tagValue);
	
	virtual void SetStringTag(FString tagName, FString tagValue);
	
	virtual void SetTags(FString json);

	virtual void SetUserId(FString userId);

	virtual void PostEvent(FString event, FString attributes);

	virtual ~PushwooshImpl() {};

	inline bool IsInitialized() { return isInitialized; }

private:
	bool isInitialized;
};
