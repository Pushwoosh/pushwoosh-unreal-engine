// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

#include "Engine.h"

#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"

#include "PushwooshSettings.generated.h"

UCLASS(Config = Pushwoosh)
class UPushwooshSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPushwooshSettings(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Config, EditAnywhere, Category=Pushwoosh, meta=(DisplayName="Pushwoosh Application Id"))
	FString ApplicationId;
	
	UPROPERTY(Config, EditAnywhere, Category=Pushwoosh, meta=(DisplayName="FCM Project Number"))
	FString FcmProjectNumber;
    
    UPROPERTY(Config, EditAnywhere, Category=Pushwoosh, meta=(DisplayName="Show push notifications in foreground"))
    bool ShowInForeground;
};
