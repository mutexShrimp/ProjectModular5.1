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

	void HandleInvocation(UModuleComponent* ModuleComponent, UModuleComponent*& PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function", meta=(Keywords="Init Rule Processer"))
	TArray<FRule> InitializeRuleProcessor(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	virtual TArray<FRule> InitializeRuleProcessor_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function", meta=(Keywords="Add Rule Processer"))
	TArray<FRule> AddRuleProcessor(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);
	virtual TArray<FRule> AddRuleProcessor_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules);


protected:
	void ResetRules();
	
	TArray<FRule> SortBothRules(TArray<FRule> DefaultRules, TArray<FRule> NewRules);

	TArray<FRule> SortRules(TArray<FRule> DefaultRules);
	
	void PrintArrayRules(TArray<TArray<FRule>> DefaultArrayRules);
	
	void PrintRules(TArray<FRule> DefaultRules);
	
	void PrintLinkedRules(ELinkedListToward LinkedListToward, TLinkedList<FRule>* LinkedList);

	TLinkedList<FRule>* LinkedListToHead(ELinkedListToward LinkedListToward);

	TLinkedList<FRule>* GetPrevLink(ELinkedListToward LinkedListToward, TLinkedList<FRule>* LinkedList);
	TLinkedList<FRule>* GetNewPrevLink(TLinkedList<FRule>* LinkedList, TLinkedList<FRule>* __HeadLinkedList);

	
	
private:

	TLinkedList<FRule>* HeadLinkedList;
	TLinkedList<FRule>* HeadLinkedList_Reverse;
	TLinkedList<FRule>* CacheLinkList;

	TArray<TArray<FRule>> FinalRules;
	TArray<FRule> FinalRuleArr;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data")
	TMap<TSubclassOf<UInvokeProcessingAbstractObject>, UInvokeProcessingAbstractObject*> InvokeProcessingObjects;
};



