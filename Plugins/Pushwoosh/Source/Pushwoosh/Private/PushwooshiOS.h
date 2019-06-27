// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

#if PLATFORM_IOS

#include "PushwooshImpl.h"

class PushwooshiOS : public PushwooshImpl
{
public:
	explicit PushwooshiOS(const FString& appId, bool foreground);

	virtual void Initialize() override;

	virtual void RegisterForPushNotifications() override;

	virtual void UnregisterForPushNotifications() override;
	
	virtual void SetIntTag(FString tagName, int tagValue) override;
	
	virtual void SetStringTag(FString tagName, FString tagValue) override;
	
	virtual void SetTags(FString json) override;

	virtual void SetUserId(FString userId) override;

	virtual void PostEvent(FString event, FString attributes) override;
private:
	FString applicationId;
    bool showInForeground;
};

#endif
