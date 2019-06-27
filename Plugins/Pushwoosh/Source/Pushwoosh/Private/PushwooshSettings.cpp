// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#include "PushwooshPrivatePCH.h"

#include "PushwooshSettings.h"

UPushwooshSettings::UPushwooshSettings(const FObjectInitializer& ObjectInitializer)
:	Super(ObjectInitializer),
	ApplicationId(""),
	FcmProjectNumber(""),
    ShowInForeground(true)
{
	
}
