// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Component/ModuleComponent.h"
#include "GameFramework/Actor.h"
#include "ModularTypes.h"
#include "RuleProcessingClassBase.generated.h"

class UInvokeProcessingAbstractObject;

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

	bool HandleInvocation(UModuleComponent* ModuleComponent, UModuleComponent*& PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function", meta=(Keywords="Init Rule Processer"),
		meta = (ToolTip =
			"The default event names implemented are as follows.\n\nModuleInitializationBegins_InvokeAll\nInitModule\nModuleInitializationCompleted_InvokeAll\n"
			))
	TArray<FRule> InitializeRuleProcessor(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	virtual TArray<FRule> InitializeRuleProcessor_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function", meta=(Keywords="Add Rule Processer"),
		meta = (ToolTip =
			"The default event names implemented are as follows.\n\nDoOnce_PreInitModule_InvokeAll\nDoOnce_PreInitModule\nDoOnce_PostInitModule\nDoOnce_PostInitModule_InvokeAll\n"
			))
	TArray<FRule> AddRuleProcessor(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	virtual TArray<FRule> AddRuleProcessor_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);


protected:
	void ResetRules();
	
	TArray<FRule> SortBothRules(TArray<FRule> DefaultRules, TArray<FRule> NewRules);

	TArray<FRule> SortRules(TArray<FRule> DefaultRules);
	
	void PrintArrayRules(TArray<TArray<FRule>> DefaultArrayRules);
	
	void PrintRules(TArray<FRule> DefaultRules);
	
	void PrintLinkedRules(TLinkedList<FRule>* LinkedList);

	int32 GetLinkedRuleIndex(const FString& RuleName);
	
	TLinkedList<FRule>* LinkedListToHead();

	TLinkedList<FRule>* GetPrevLink(TLinkedList<FRule>* LinkedList);

	int32 GetIndexByRuleName(FRulesOrderProcessing RulesOrderProcessing, TArray<FRulesOrderProcessing> RulesOrderProcessingArr, TArray<int32> IgnoreIndexes);

	FRulesOrderProcessing GetRuleByIndex(int32 Index, TArray<FRulesOrderProcessing> RulesOrderProcessingArr);

	TArray<FRule> PreorderTraversalRules(FRulesOrderProcessing RulesOrderProcessing, TArray<FRulesOrderProcessing> RulesOrderProcessingArr, TArray<int32>& IgnoreIndexes);
	
private:

	TLinkedList<FRule>* HeadLinkedList;
	
	TArray<TArray<TTuple<int32, FRule>>> FinalRulesTuple;
	TArray<FRule> FinalRuleArr;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data")
	TMap<TSubclassOf<UInvokeProcessingAbstractObject>, UInvokeProcessingAbstractObject*> InvokeProcessingObjects;
};



