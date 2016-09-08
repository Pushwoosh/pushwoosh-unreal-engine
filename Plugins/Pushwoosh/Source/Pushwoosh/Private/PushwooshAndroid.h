// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

#if PLATFORM_ANDROID

#include "PushwooshImpl.h"

#include <jni.h>

class PushwooshAndroid : public PushwooshImpl
{
public:
	PushwooshAndroid(const FString& appId, const FString& gcmPN);
	~PushwooshAndroid();

	virtual void Initialize() override;

	virtual void RegisterForPushNotifications() override;

	virtual void UnregisterForPushNotifications() override;

	virtual void SetIntTag(FString tagName, int tagValue) override;
	
	virtual void SetStringTag(FString tagName, FString tagValue) override;
	
	virtual void SetTags(FString json) override;

private:
	FString applicationId;
	FString gcmProjectNumber;

	jobject pluginObject;

	jmethodID androidThunkJava_Initialize;
	jmethodID androidThunkJava_RegisterForPushNotifications;
	jmethodID androidThunkJava_UnregisterForPushNotifications;
	jmethodID androidThunkJava_SetTags;

	void InitializeJavaInterface();
	void CheckJavaObject(JNIEnv* env, void* p, const char* signature);
};

#endif
