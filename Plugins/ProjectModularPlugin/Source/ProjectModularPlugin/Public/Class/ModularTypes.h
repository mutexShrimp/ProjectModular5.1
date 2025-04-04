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

struct FRulesOrderProcessing
{
private:
	FString RuleName;
	FRule Rule;
	TArray<FRulesOrderProcessing> PreRules;
	TArray<FRulesOrderProcessing> PostRules;
	TArray<FRulesOrderProcessing> ContainsRules;

public:
	void SetRule(FRule NewRule)
	{
		Rule = NewRule;
		RuleName = NewRule.RuleName;
	}

	FRule GetRule()
	{
		return Rule;
	}

	FString GetRuleName()
	{
		return Rule.RuleName;
	}
	
	void AddPreRule(FRule NewRule)
	{
		FRulesOrderProcessing RulesOrderProcessing;
		RulesOrderProcessing.SetRule(NewRule);
		PreRules.Add(RulesOrderProcessing);
		ContainsRules.Add(RulesOrderProcessing);
	}

	void AddPreRule(FRulesOrderProcessing RulesOrderProcessing)
	{
		PreRules.Add(RulesOrderProcessing);
		ContainsRules.Add(RulesOrderProcessing);
	}

	TArray<FRulesOrderProcessing> GetPreRules()
	{
		return PreRules;
	}
	
	void AddPostRule(FRule NewRule)
	{
		FRulesOrderProcessing RulesOrderProcessing;
		RulesOrderProcessing.SetRule(NewRule);
		PostRules.Add(RulesOrderProcessing);
		ContainsRules.Add(RulesOrderProcessing);
	}
	
	void AddPostRule(FRulesOrderProcessing RulesOrderProcessing)
	{
		PostRules.Add(RulesOrderProcessing);
		ContainsRules.Add(RulesOrderProcessing);
	}

	TArray<FRulesOrderProcessing> GetPostRules()
	{
		return PostRules;
	}
	
};

UCLASS()
class PROJECTMODULARPLUGIN_API UModularTypes : public UObject
{
	GENERATED_BODY()
	
};
