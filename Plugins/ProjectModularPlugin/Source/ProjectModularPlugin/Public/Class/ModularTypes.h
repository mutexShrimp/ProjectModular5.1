// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularTypes.generated.h"

UENUM(BlueprintType)
enum class ERuleTriggerType : uint8
{
	Pre UMETA(DisplayName = "Pre"),
	Post UMETA(DisplayName = "Post")
};

UENUM(BlueprintType)
enum class ELinkedListToward : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward")
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RuleName = TEXT("MyNewRule");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TriggingDependencies = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInvokeProcessingAbstractObject* InvokeProcessingObject = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERuleTriggerType TriggerType = ERuleTriggerType::Post;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TriggerLevel = 0;

	
};

UCLASS()
class PROJECTMODULARPLUGIN_API UModularTypes : public UObject
{
	GENERATED_BODY()
	
};
