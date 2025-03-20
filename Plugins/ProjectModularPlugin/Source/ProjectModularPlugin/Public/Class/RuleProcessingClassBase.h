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

UENUM(BlueprintType)
enum class ELinkedListToward : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward")
};

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
	TArray<FRule> InitializeRuleProcessor(UModuleComponent* ModuleComponent);
	virtual TArray<FRule> InitializeRuleProcessor_Implementation(UModuleComponent* ModuleComponent);
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function", meta=(Keywords="Add Rule Processer"))
	TArray<FRule> AddRuleProcessor(UModuleComponent* ModuleComponent);
	virtual TArray<FRule> AddRuleProcessor_Implementation(UModuleComponent* ModuleComponent);

protected:
	TArray<FRule> SortBothRules(TArray<FRule> DefaultRules, TArray<FRule> NewRules);

	TArray<FRule> SortRules(TArray<FRule> DefaultRules);

	void PrintArrayRules(TArray<TArray<FRule>> DefaultArrayRules);
	
	void PrintRules(TArray<FRule> DefaultRules);
	
	void PrintLinkedRules(ELinkedListToward LinkedListToward, TLinkedList<FRule>* LinkedList);

	TLinkedList<FRule>* LinkedListToHead(ELinkedListToward LinkedListToward);

	TLinkedList<FRule>* GetPrevLink(ELinkedListToward LinkedListToward, TLinkedList<FRule>* LinkedList);
	TLinkedList<FRule>* GetNewPrevLink(TLinkedList<FRule>* LinkedList, TLinkedList<FRule>* __HeadLinkedList);
	
private:
	TArray<FRule> Rules;

	TLinkedList<FRule>* HeadLinkedList;
	TLinkedList<FRule>* HeadLinkedList_Reverse;
	TLinkedList<FRule>* CacheLinkList;

	TArray<TArray<FRule>> FinalRules;
};



