// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

#include "PushwooshSettings.generated.h"

UCLASS(Config = Pushwoosh)
class UPushwooshSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPushwooshSettings(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Config, EditAnywhere, Category=Pushwoosh, meta=(DisplayName="Pushwoosh Application Id"))
	FString ApplicationId;
	
	UPROPERTY(Config, EditAnywhere, Category=Pushwoosh, meta=(DisplayName="GCM Project Number"))
	FString GcmProjectNumber;
};
