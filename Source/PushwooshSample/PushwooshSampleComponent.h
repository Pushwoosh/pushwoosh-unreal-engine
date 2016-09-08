// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "PushwooshSampleComponent.generated.h"


UCLASS( ClassGroup=(PushwooshSample), meta=(BlueprintSpawnableComponent) )
class PUSHWOOSHSAMPLE_API UPushwooshSampleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPushwooshSampleComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	void PushRegistrationSucceeded_Handler(FString token);
	void PushRegistrationError_Handler(FString error);
	void PushAccepted_Handler(FString data);
};
