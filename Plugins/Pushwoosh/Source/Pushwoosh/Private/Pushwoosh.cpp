// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#include "PushwooshPrivatePCH.h"

#include "PushwooshImpl.h"
#include "PushwooshiOS.h"
#include "PushwooshAndroid.h"
#include "PushwooshSettings.h"

#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FPushwooshModule"

DEFINE_LOG_CATEGORY(LogPushwoosh);

FPushwooshRegistrationSuccessDelegate FPushwooshModule::PushRegistrationSucceeded;

FPushwooshRegistrationErrorDelegate FPushwooshModule::PushRegistrationError;

FPushwooshPushAcceptedDelegate FPushwooshModule::PushAccepted;

void FPushwooshModule::StartupModule()
{
	UE_LOG(LogPushwoosh, Log, TEXT("Pushwoosh module started"));

	if (ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		settingsModule->RegisterSettings("Project", "Plugins", "Pushwoosh",
										 LOCTEXT("RuntimeSettingsName", "Pushwoosh"),
										 LOCTEXT("RuntimeSettingsDescription", "Configure the Pushwoosh plugin (these settings are saved in DefaultPushwoosh.ini)"),
										 GetMutableDefault<UPushwooshSettings>()
										 );
	}

	const UPushwooshSettings* pushwooshSettings = GetDefault<UPushwooshSettings>();

	#if PLATFORM_IOS
		pImpl = new PushwooshiOS(pushwooshSettings->ApplicationId);
	#elif PLATFORM_ANDROID
		pImpl = new PushwooshAndroid(pushwooshSettings->ApplicationId, pushwooshSettings->GcmProjectNumber);
	#else
		pImpl = new PushwooshImpl();
	#endif
}

void FPushwooshModule::ShutdownModule()
{
	UE_LOG(LogPushwoosh, Log, TEXT("Pushwoosh module shut down"));

	delete pImpl;
}

void FPushwooshModule::Initialize()
{
	if (pImpl->IsInitialized())
	{
		UE_LOG(LogPushwoosh, Warning, TEXT("Pushwoosh is already initialized"));
		return;
	}
	
	pImpl->Initialize();
}

void FPushwooshModule::RegisterForPushNotifications()
{
	pImpl->RegisterForPushNotifications();
}

void FPushwooshModule::UnregisterForPushNotifications()
{
	pImpl->UnregisterForPushNotifications();
}

void FPushwooshModule::SetIntTag(FString tagName, int tagValue)
{
	pImpl->SetIntTag(tagName, tagValue);
}

void FPushwooshModule::SetStringTag(FString tagName, FString tagValue)
{
	pImpl->SetStringTag(tagName, tagValue);
}

void FPushwooshModule::SetTags(FString json)
{
	pImpl->SetTags(json);
}

void FPushwooshModule::SetUserId(FString userId)
{
	pImpl->SetUserId(userId);
}

void FPushwooshModule::PostEvent(FString event, FString attributes)
{
	pImpl->PostEvent(event, attributes);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPushwooshModule, Pushwoosh)
