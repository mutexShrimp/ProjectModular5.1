// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularTypes.generated.h"

UENUM(BlueprintType)
enum class ERuleTriggerType : uint8
{
	Pre UMETA(DisplayName = "Pre"),
	Post UMETA(DisplayName = "Post")
};


class UInvokeProcessingAbstractObject;

USTRUCT(BlueprintType)
struct FRule
{
	GENERATED_USTRUCT_BODY()
	
	FRule() = default;
	
	FRule(FString RuleName, UInvokeProcessingAbstractObject* InvokeProcessingObject = nullptr, FString TriggingDependencies = TEXT(""), ERuleTriggerType TriggerType = ERuleTriggerType::Post, int32 TriggerLevel = 0)
	{
		this->RuleName = RuleName;
		this->InvokeProcessingObject = InvokeProcessingObject;
		this->TriggingDependencies = TriggingDependencies;
		this->TriggerType = TriggerType;
		this->TriggerLevel = TriggerLevel;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FString RuleName = TEXT("MyNewRule");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FString TriggingDependencies = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	UInvokeProcessingAbstractObject* InvokeProcessingObject = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	ERuleTriggerType TriggerType = ERuleTriggerType::Post;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 TriggerLevel = 0;
	
};

UCLASS()
class PROJECTMODULARPLUGIN_API UModularTypes : public UObject
{
	GENERATED_BODY()
	
};
