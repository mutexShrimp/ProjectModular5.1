// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ModuleComponent.h"
#include "GameFramework/Actor.h"
#include "RuleProcessingClassBase.generated.h"

class UInvokeProcessingAbstractObject;

UENUM(BlueprintType)
enum class ERuleTriggerType : uint8
{
	Pre UMETA(DisplayName = "Pre"),
	Post UMETA(DisplayName = "Post")
};

USTRUCT(BlueprintType)
struct FRule
{
	GENERATED_USTRUCT_BODY()
	
	FRule() = default;
	
	FRule(FString RuleName, FString TriggingDependencies = TEXT(""), UInvokeProcessingAbstractObject* InvokeProcessingObject = nullptr, ERuleTriggerType TriggerType = ERuleTriggerType::Post, int32 TriggerLevel = 0)
	{
		this->RuleName = RuleName;
		this->TriggingDependencies = TriggingDependencies;
		this->InvokeProcessingObject = InvokeProcessingObject;
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
class PROJECTMODULARPLUGIN_API ARuleProcessingClassBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuleProcessingClassBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleInvocation(UModuleComponent* ModuleComponent);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function", meta=(Keywords="Init Rule Processer"))
	TArray<FRule> InitializeRuleProcessor();
	virtual TArray<FRule> InitializeRuleProcessor_Implementation();
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function", meta=(Keywords="Add Rule Processer"))
	void AddRuleProcessor(TArray<FRule>& NewRules);
	virtual void AddRuleProcessor_Implementation(TArray<FRule>& NewRules);
	
private:
	TArray<FRule> Rules;
	
};



