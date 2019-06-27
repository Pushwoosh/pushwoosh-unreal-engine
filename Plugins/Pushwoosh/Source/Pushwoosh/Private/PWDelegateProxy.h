// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#if PLATFORM_IOS

@interface PWDelegateProxy : NSObject

+ (instancetype)sharedProxy;

- (void)onListenersReady;

- (void)onRegisterForPushNotifications;

- (void)onDidRegisterForRemoteNotificationsWithDeviceToken:(NSString *)token;

- (void)onDidFailToRegisterForRemoteNotificationsWithError:(NSString *)error;

- (void)onPushAccepted:(NSString *)data;

- (void)onPushReceived:(NSString *)data;

@end

#endif
