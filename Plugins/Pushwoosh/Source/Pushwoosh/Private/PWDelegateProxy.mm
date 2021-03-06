// Copyright 2016 Pushwoosh Inc. All Rights Reserved.
#ifdef PLATFORM_IOS
#if PLATFORM_IOS

#include "PushwooshPrivatePCH.h"

#import "PWDelegateProxy.h"

@interface PWDelegateProxy()

@property (nonatomic, strong) NSString *startPushData;
@property (nonatomic, strong) NSString *receivedStartPushData;
@property (nonatomic, assign) BOOL listenersReady;
@property (atomic, assign) BOOL registeringForPushes;

@end


@implementation PWDelegateProxy

+ (instancetype)sharedProxy
{
	static PWDelegateProxy *instance = nil;
	static dispatch_once_t pred;
	
	dispatch_once(&pred, ^{
		instance = [PWDelegateProxy new];
	});
	
	return instance;
}

- (void)onListenersReady
{
	@synchronized (self)
	{
		if (self.startPushData) {
			[self dispatchPush:self.startPushData];
            self.startPushData = nil;
		}
        if (self.receivedStartPushData) {
            [self dispatchPushReceived:self.receivedStartPushData];
            self.receivedStartPushData = nil;
        }
		
		self.listenersReady = YES;
	}
}

- (void)onRegisterForPushNotifications
{
	self.registeringForPushes = YES;
}

- (void)onDidRegisterForRemoteNotificationsWithDeviceToken:(NSString *)token
{
	if (self.registeringForPushes)
	{
		const FString fToken = FString(UTF8_TO_TCHAR([token UTF8String]));
		FPushwooshModule::PushRegistrationSucceeded.Broadcast(fToken);
		
		self.registeringForPushes = NO;
	}
}

- (void)onDidFailToRegisterForRemoteNotificationsWithError:(NSString *)error
{
	if (self.registeringForPushes)
	{
		const FString fError = FString(UTF8_TO_TCHAR([error UTF8String]));
		FPushwooshModule::PushRegistrationError.Broadcast(fError);
		
		self.registeringForPushes = NO;
	}
}

- (void)onPushAccepted:(NSString *)data
{
	@synchronized (self)
	{
		if (!self.listenersReady)
		{
			self.startPushData = data;
		}
		else
		{
			[self dispatchPush:data];
		}
	}
}

- (void)onPushReceived:(NSString *)data
{
    @synchronized (self)
    {
        if (!self.listenersReady)
        {
            self.receivedStartPushData = data;
        }
        else
        {
            [self dispatchPushReceived:data];
        }
    }
}

- (void)dispatchPush:(NSString*)data
{
	const FString fData = FString(UTF8_TO_TCHAR([data UTF8String]));
	FPushwooshModule::PushAccepted.Broadcast(fData);
}

- (void)dispatchPushReceived:(NSString*)data
{
    const FString fData = FString(UTF8_TO_TCHAR([data UTF8String]));
    FPushwooshModule::PushReceived.Broadcast(fData);
}

@end

#endif
#endif
