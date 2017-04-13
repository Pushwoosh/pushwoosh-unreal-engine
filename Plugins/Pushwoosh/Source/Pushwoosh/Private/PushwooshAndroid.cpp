// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#include "PushwooshPrivatePCH.h"

#include "PushwooshAndroid.h"

#if PLATFORM_ANDROID

#include "Android/AndroidJNI.h"
#include "AndroidApplication.h"

extern "C" {

JNIEXPORT void JNICALL pw_JavaCallback_onPushRegistered(JNIEnv* env, jclass clazz, jstring token)
{
	const char *nativeString = env->GetStringUTFChars(token, 0);
	FString fTokenStr = nativeString;

	UE_LOG(LogPushwoosh, Log, TEXT("Registered for push notifications: %s"), *fTokenStr);

	env->ReleaseStringUTFChars(token, nativeString);

	FPushwooshModule::PushRegistrationSucceeded.Broadcast(fTokenStr);
}

JNIEXPORT void JNICALL pw_JavaCallback_onPushRegistrationError(JNIEnv* env, jclass clazz, jstring message)
{
	const char *nativeString = env->GetStringUTFChars(message, 0);
	FString fErrorStr = nativeString;

	UE_LOG(LogPushwoosh, Warning, TEXT("Failed to register for push notifications: %s"), *fErrorStr);

	env->ReleaseStringUTFChars(message, nativeString);

	FPushwooshModule::PushRegistrationError.Broadcast(fErrorStr);
}

JNIEXPORT void JNICALL pw_JavaCallback_onPushAccepted(JNIEnv* env, jclass clazz, jstring payload)
{
	const char *nativeString = env->GetStringUTFChars(payload, 0);
	FString fPayloadStr = nativeString;

	UE_LOG(LogPushwoosh, Log, TEXT("Push accepted: %s"), *fPayloadStr);

	env->ReleaseStringUTFChars(payload, nativeString);

	FPushwooshModule::PushAccepted.Broadcast(fPayloadStr);
}

}

PushwooshAndroid::PushwooshAndroid(const FString& appId, const FString& gcmPN)
:	applicationId(appId),
	gcmProjectNumber(gcmPN)
{
	InitializeJavaInterface();
}

PushwooshAndroid::~PushwooshAndroid()
{
	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		if (pluginObject)
		{
			env->DeleteGlobalRef(pluginObject);
		}
	}
}

void PushwooshAndroid::Initialize() 
{
	PushwooshImpl::Initialize();

	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		jstring jStrAppId = env->NewStringUTF(TCHAR_TO_ANSI(*applicationId));
		jstring jStrGcmProject = env->NewStringUTF(TCHAR_TO_ANSI(*gcmProjectNumber));

		env->CallVoidMethod(pluginObject, androidThunkJava_Initialize, jStrAppId, jStrGcmProject);
	}
}

void PushwooshAndroid::RegisterForPushNotifications()
{
	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		env->CallVoidMethod(pluginObject, androidThunkJava_RegisterForPushNotifications);
	}
}

void PushwooshAndroid::UnregisterForPushNotifications() 
{
	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		env->CallVoidMethod(pluginObject, androidThunkJava_UnregisterForPushNotifications);
	}
}

void PushwooshAndroid::SetIntTag(FString tagName, int tagValue)
{
	FString json = FString::Printf(TEXT("{ \"%s\" : %d }"), *tagName, tagValue); 
	SetTags(json);
}

void PushwooshAndroid::SetStringTag(FString tagName, FString tagValue)
{
	FString json = FString::Printf(TEXT("{ \"%s\" : \"%s\" }"), *tagName, *tagValue); 
	SetTags(json);
}

void PushwooshAndroid::SetTags(FString json)
{
	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		jstring jStrJson= env->NewStringUTF(TCHAR_TO_UTF8(*json));
		env->CallVoidMethod(pluginObject, androidThunkJava_SetTags, jStrJson);
	}
}

void PushwooshAndroid::SetUserId(FString userId)
{
	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		jstring jStrUserId= env->NewStringUTF(TCHAR_TO_UTF8(*userId));
		env->CallVoidMethod(pluginObject, androidThunkJava_SetUserId, jStrUserId);
	}
}

void PushwooshAndroid::PostEvent(FString event, FString attributes)
{
	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		jstring jStrEvent = env->NewStringUTF(TCHAR_TO_UTF8(*event));
		jstring jStrAttributes = env->NewStringUTF(TCHAR_TO_UTF8(*attributes));
		env->CallVoidMethod(pluginObject, androidThunkJava_PostEvent, jStrEvent, jStrAttributes);
	}
}

void PushwooshAndroid::InitializeJavaInterface()
{
	UE_LOG(LogPushwoosh, Log, TEXT("Initializing Java interface..."));

	if (JNIEnv* env = FAndroidApplication::GetJavaEnv())
	{
		jclass pluginClass = FAndroidApplication::FindJavaClass("com/pushwoosh/ueplugin/PushwooshPlugin");
		CheckJavaObject(env, pluginClass, "com/pushwoosh/ueplugin/PushwooshPlugin");

		jmethodID androidThunkJava_getInstance = FJavaWrapper::FindStaticMethod(env, pluginClass, "getInstance", "()Lcom/pushwoosh/ueplugin/PushwooshPlugin;", false);
		androidThunkJava_Initialize = FJavaWrapper::FindMethod(env, pluginClass, "initialize", "(Ljava/lang/String;Ljava/lang/String;)V", false);
		androidThunkJava_RegisterForPushNotifications = FJavaWrapper::FindMethod(env, pluginClass, "registerForPushNotifications", "()V", false);
		androidThunkJava_UnregisterForPushNotifications = FJavaWrapper::FindMethod(env, pluginClass, "unregisterForPushNotifications", "()V", false);
		androidThunkJava_SetTags = FJavaWrapper::FindMethod(env, pluginClass, "setTags", "(Ljava/lang/String;)V", false);
		androidThunkJava_SetUserId = FJavaWrapper::FindMethod(env, pluginClass, "setUserId", "(Ljava/lang/String;)V", false);
		androidThunkJava_PostEvent = FJavaWrapper::FindMethod(env, pluginClass, "postEvent", "(Ljava/lang/String;Ljava/lang/String;)V", false);

		jobject localPluginObject = env->CallStaticObjectMethod(pluginClass, androidThunkJava_getInstance);
		CheckJavaObject(env, localPluginObject, "PushwooshPlugin instance");

		pluginObject = env->NewGlobalRef(localPluginObject);
		env->DeleteLocalRef(localPluginObject);

		static const JNINativeMethod jnm[] = {
			{"native_onPushRegistered", "(Ljava/lang/String;)V", (void *)&pw_JavaCallback_onPushRegistered},
			{"native_onPushRegistrationError", "(Ljava/lang/String;)V", (void *)&pw_JavaCallback_onPushRegistrationError},
			{"native_onPushAccepted", "(Ljava/lang/String;)V", (void *)&pw_JavaCallback_onPushAccepted }
		};

		env->RegisterNatives(pluginClass, jnm, sizeof(jnm) / sizeof(jnm[0]));

		UE_LOG(LogPushwoosh, Log, TEXT("Java interface initialized"));
	}
	else
	{
		UE_LOG(LogPushwoosh, Error, TEXT("Failed to get Java Environment"));
	}
}

void PushwooshAndroid::CheckJavaObject(JNIEnv* env, void* p, const char* desc)
{
	FString str = desc;
	if (!p)
	{
		jthrowable exc = env->ExceptionOccurred();
		if (exc)
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
		
		UE_LOG(LogPushwoosh, Error, TEXT("Failed to reference %s"), *str);
	}
	else
	{
		UE_LOG(LogPushwoosh, Log, TEXT("%s sucessfully referenced"), *str);
	}
}

#endif
