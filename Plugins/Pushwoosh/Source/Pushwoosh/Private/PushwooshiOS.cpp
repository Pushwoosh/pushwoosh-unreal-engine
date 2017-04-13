// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#include "PushwooshPrivatePCH.h"

#include "PushwooshiOS.h"

#if PLATFORM_IOS

#import "PWDelegateProxy.h"

#import <Pushwoosh/Pushwoosh.h>
#import <UserNotifications/UserNotifications.h>

NSString *g_startPushData = nil;
NSObject *g_lock = [NSObject new];
BOOL g_initialized = NO;

@implementation UIApplication(PWUnreal)

- (NSObject<PushNotificationDelegate> *)getPushwooshDelegate {
	return (NSObject<PushNotificationDelegate> *)[UIApplication sharedApplication];
}

- (BOOL) pushwooshUseRuntimeMagic {
	return YES;
}

- (void) onDidRegisterForRemoteNotificationsWithDeviceToken:(NSString *)token
{
	const FString fToken = FString(UTF8_TO_TCHAR([token UTF8String]));
	UE_LOG(LogPushwoosh, Log, TEXT("Registered with push token: %s"), *fToken);

	[[PWDelegateProxy sharedProxy] onDidRegisterForRemoteNotificationsWithDeviceToken:token];
}

- (void) onDidFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	NSString *description = [error description];
	
	const FString fError = FString(UTF8_TO_TCHAR([description UTF8String]));
	UE_LOG(LogPushwoosh, Warning, TEXT("Failed to register for push notifications: %s"), *fError);

	[[PWDelegateProxy sharedProxy] onDidFailToRegisterForRemoteNotificationsWithError:description];
}

- (void) onPushAccepted:(PushNotificationManager *)pushManager withNotification:(NSDictionary *)pushNotification onStart:(BOOL)onStart
{
	NSMutableDictionary *pushNotificationMutable = [pushNotification mutableCopy];
	[pushNotificationMutable setObject:[NSNumber numberWithBool:onStart] forKey:@"onStart"];
	
	NSData *jsonData = [NSJSONSerialization dataWithJSONObject:pushNotificationMutable options:0 error:nil];
	NSString *pushStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];

	const FString fPushPayload = FString(UTF8_TO_TCHAR([pushStr UTF8String]));
	UE_LOG(LogPushwoosh, Log, TEXT("Push notification opened: %s"), *fPushPayload);

	[[PWDelegateProxy sharedProxy] onPushAccepted:pushStr];
}

@end

PushwooshiOS::PushwooshiOS(const FString& appId)
:	applicationId(appId)
{
	
}

void PushwooshiOS::Initialize() 
{
	PushwooshImpl::Initialize();

	UE_LOG(LogPushwoosh, Log, TEXT("Initializing with applicationId: %s"), *applicationId);
	
	[[PWDelegateProxy sharedProxy] onListenersReady];
	 
	[PushNotificationManager initializeWithAppCode:applicationId.GetNSString() appName:nil];
	[PushNotificationManager pushManager].delegate = (NSObject<PushNotificationDelegate> *)[UIApplication sharedApplication];
	[UNUserNotificationCenter currentNotificationCenter].delegate = [PushNotificationManager pushManager].notificationCenterDelegate;

	[[PushNotificationManager pushManager] sendAppOpen];
}

void PushwooshiOS::RegisterForPushNotifications()
{
	[[PWDelegateProxy sharedProxy] onRegisterForPushNotifications];
	[[PushNotificationManager pushManager] registerForPushNotifications];
}

void PushwooshiOS::UnregisterForPushNotifications() 
{
	[[PushNotificationManager pushManager] unregisterForPushNotifications];
}

void PushwooshiOS::SetIntTag(FString tagName, int tagValue)
{
	[[PushNotificationManager pushManager] setTags: @{ tagName.GetNSString() : @(tagValue) }];
}

void PushwooshiOS::SetStringTag(FString tagName, FString tagValue)
{
	[[PushNotificationManager pushManager] setTags: @{ tagName.GetNSString() : tagValue.GetNSString() }];
}

void PushwooshiOS::SetTags(FString json)
{
	NSData *data = [json.GetNSString() dataUsingEncoding:NSUTF8StringEncoding];
	NSError *error = nil;
	NSDictionary *dictionary = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];

	if (!dictionary) {
		UE_LOG(LogPushwoosh, Error, TEXT("SetTags failed, invalid json: %s, %s"), *json, UTF8_TO_TCHAR([[error description] UTF8String]));
		return;
	}
	
	[[PushNotificationManager pushManager] setTags:dictionary];
}

void PushwooshiOS::SetUserId(FString userId)
{
	[[PWInAppManager sharedManager] setUserId:userId.GetNSString()];
}

void PushwooshiOS::PostEvent(FString event, FString attributes)
{
	attributes.Trim();
	if (attributes.IsEmpty()) {
		attributes = "{}";
	}

	NSData *data = [attributes.GetNSString() dataUsingEncoding:NSUTF8StringEncoding];
	NSError *error = nil;
	NSDictionary *dictionary = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];

	if (!dictionary) {
		UE_LOG(LogPushwoosh, Error, TEXT("PostEvent failed, invalid json: %s, %s"), *attributes, UTF8_TO_TCHAR([[error description] UTF8String]));
		return;
	}
	
	[[PWInAppManager sharedManager] postEvent:event.GetNSString() withAttributes:dictionary];
}

#endif
