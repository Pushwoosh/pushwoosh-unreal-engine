## FPushwooshModule

| Header | [Pushwoosh.h](../Source/Pushwoosh/Public/Pushwoosh.h) |
| ------ | ----------------------------------------------------- |

```cpp
class FPushwooshModule : public IModuleInterface
```

Provides methods to receive and handle push notifications for iOS and Android.

Example:

```cpp
void YourComponent::InitPushwoosh()
{
	if (FPushwooshModule::IsAvailable())
	{
		FPushwooshModule::PushRegistrationSucceeded.AddUObject(this, &YourComponent::PushRegistrationSucceeded_Handler);
		FPushwooshModule::PushRegistrationError.AddUObject(this, &YourComponent::PushRegistrationError_Handler);
		FPushwooshModule::PushAccepted.AddUObject(this, &YourComponent::PushAccepted_Handler);
		
		FPushwooshModule& pushwoosh = FPushwooshModule::Get();
		pushwoosh.Initialize();
		pushwoosh.RegisterForPushNotifications();
	}
}

void YourComponent::PushRegistrationSucceeded_Handler(FString token)
{
  // TODO: handle successful registration
}

void YourComponent::PushRegistrationError_Handler(FString error)
{
  // TODO: handle push registration error
}

void YourComponent::PushAccepted_Handler(FString data)
{
  // TODO: handle push open
}
```

---

## Fields

[static FPushwooshRegistrationSuccessDelegate PushRegistrationSucceeded](#pushregistrationsucceeded)  
[static FPushwooshRegistrationErrorDelegate PushRegistrationError](#pushregistrationerror)  
[static FPushwooshPushAcceptedDelegate PushAccepted](#pushaccepted)  

## Public Methods

[void Initialize()](#initialize)  
[void RegisterForPushNotifications()](#registerforpushnotifications)  
[void UnregisterForPushNotifications()](#unregisterforpushnotifications)  
[void SetIntTag(FString tagName, int tagValue)](setinttag)  
[void SetStringTag(FString tagName, FString tagValue)](#setstringtag)  
[void SetTags(FString json)](settags)  
[void SetUserId(FString userId)](setuserid)  
[void PostEvent(FString event, FString attributes)](postevent)  

---

### PushRegistrationSucceeded

```cpp
static FPushwooshRegistrationSuccessDelegate PushRegistrationSucceeded
```

Push registration success event. Is triggered after successful RegisterForPushNotifications() call.

Parameters:
 * **FString pushToken** - push notification token.


### PushRegistrationError

```cpp
static FPushwooshRegistrationErrorDelegate PushRegistrationError
```

Push registration error event. Is triggered if RegisterForPushNotifications() method fails.

Parameters:
 * **FString error** - error description.


### PushAccepted

```cpp
static FPushwooshPushAcceptedDelegate PushAccepted
```

Push notification accept event. Is triggered when user taps on push notification.

Parameters:
 * **FString json** - push notification payload in JSON format.



### Initialize

```cpp
void Initialize()
```

Initializes plugin with specified Settings (Edit -> Project Settings... -> Plugins -> Pushwoosh).
Tells plugin that client is ready to receive PushRegistrationSucceeded, PushRegistrationError and PushAccepted Delegate events.
Sends application open statistics.
Should be called on every application start before invoking any other module methods.


### RegisterForPushNotifications

```cpp
void RegisterForPushNotifications()
```

Request push notification registration. This will result in notification permisstion dialog on iOS.
If registration succeeds PushRegistrationSucceeded is broadcasted.
If registration fails PushRegistrationError is broadcasted.

Note: On iOS it is recommened to set 'UIBackgroundModes: remote-notification' in Into.plist 
	  to be able to get push token and register for push notifications regardless of user choice.
	  Also if 'UIBackgroundModes: remote-notification' is not set and user disables push notifications no callback will be invoked.


### UnregisterForPushNotifications

```cpp
void UnregisterForPushNotifications()
```

Unregisters device from push notifications and tells server that device is no longer subscribed.


### SetIntTag

```cpp
void SetIntTag(FString tagName, int tagValue)
```

Associates device with given (tagName, tagValue) pair.


### SetStringTag

```cpp
void SetStringTag(FString tagName, FString tagValue)
```

Associates device with given (tagName, tagValue) pair.


### SetTags

```cpp
void SetTags(FString json)
```

Associates device with given tags in JSON format. Multiple tags can be specified in a single request.

Example:
```cpp
FPushwooshModule& pushwoosh = FPushwooshModule::Get();
pushwoosh.SetTags("{ \"intTag\" : 1, \"stringTag\" : \"2\", \"listTag\" : [ \"3\", \"4\", \"5\" ] }");
```


### SetUserId

```cpp
void SetUserId(FString userId);
```

Set User indentifier. This could be Facebook ID, username or email, or any other user ID.
This allows data and events to be matched across multiple user devices.


### PostEvent

```cpp
void PostEvent(FString event, FString attributes)
```

Post events for In-App Messages. This can trigger In-App message display as specified in Pushwoosh Control Panel.
@param event name of the event
@param attributes JSON object with additional event parameters

Example:
```cpp
FPushwooshModule& pushwoosh = FPushwooshModule::Get();
pushwoosh.PostEvent("buttonPressed", "{ \"buttonNumber\" : \"4\", \"buttonLabel\" : \"Banner\" }");
```
