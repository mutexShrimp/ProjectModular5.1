// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/RuleProcessingClassBase.h"

#include "Class/ModuleClassInterface.h"
#include "Class/InvokeProcessing/InvokeProcessing_AllModule.h"
#include "Class/InvokeProcessing/InvokeProcessing_AllModule_Once.h"
#include "Class/InvokeProcessing/InvokeProcessing_CurModule.h"
#include "Class/InvokeProcessing/InvokeProcessing_CurModule_Once.h"
#include "Library/Blueprint/BlueprintModuleLibrary.h"

// Sets default values
ARuleProcessingClassBase::ARuleProcessingClassBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARuleProcessingClassBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARuleProcessingClassBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARuleProcessingClassBase::HandleInvocation(UModuleComponent* ModuleComponent)
{
	// Sort
	TArray<FRule> DefaultRules = InitializeRuleProcessor(ModuleComponent);

	TArray<FRule> AddedRules = AddRuleProcessor(ModuleComponent);

	SortBothRules(DefaultRules, AddedRules);
	
	for (FRule Rule : DefaultRules)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rule Processing Function Implemented %s"), *Rule.RuleName);
	}
	
	// Invoke
	for (FRule DefaultRule : DefaultRules)
	{
		DefaultRule.InvokeProcessingObject->InvokeProcessing(ModuleComponent);
	}
	
}

TArray<FRule> ARuleProcessingClassBase::InitializeRuleProcessor_Implementation(UModuleComponent* ModuleComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("InitializeRuleProcessor Function is Invoked"));

	TArray<FRule> NewRules;
	
	UInvokeProcessing_AllModule* InvokeProcessingObject_AllModule = NewObject<UInvokeProcessing_AllModule>(this);
	
	NewRules.Add(FRule(TEXT("ModuleInitializationBegins_InvokeAll"), InvokeProcessingObject_AllModule));

	NewRules.Add(FRule(TEXT("ModuleInitializationCompleted_InvokeAll"), InvokeProcessingObject_AllModule));

	return NewRules;
}

TArray<FRule> ARuleProcessingClassBase::AddRuleProcessor_Implementation(UModuleComponent* ModuleComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("AddRuleProcessor Function is Invoked"));

	TArray<FRule> NewRules;

	UInvokeProcessing_CurModule* InvokeProcessingObject_CurrentModule = NewObject<UInvokeProcessing_CurModule>(this);
	UInvokeProcessing_CurModule_Once* InvokeProcessingObject_CurrentModule_DoOnce = NewObject<UInvokeProcessing_CurModule_Once>(this);
	UInvokeProcessing_AllModule_Once* InvokeProcessingObject_AllModule_DoOnce = NewObject<UInvokeProcessing_AllModule_Once>(this);
	
	NewRules.Add(FRule(TEXT("InitModule"), InvokeProcessingObject_CurrentModule, TEXT("PreInitialEvent_InvokeAll"), ERuleTriggerType::Post, 10));

	NewRules.Add(FRule(TEXT("DoOnce_PreInitModule_InvokeAll"), InvokeProcessingObject_AllModule_DoOnce, TEXT("InitModule"), ERuleTriggerType::Pre, 10));

	NewRules.Add(FRule(TEXT("DoOnce_PreInitModule"), InvokeProcessingObject_CurrentModule_DoOnce, TEXT("InitModule"), ERuleTriggerType::Pre, 11));

	NewRules.Add(FRule(TEXT("DoOnce_PostInitModule_InvokeAll"), InvokeProcessingObject_AllModule_DoOnce, TEXT("InitModule"), ERuleTriggerType::Post, 10));

	NewRules.Add(FRule(TEXT("DoOnce_PostInitModule"), InvokeProcessingObject_CurrentModule_DoOnce, TEXT("InitModule"), ERuleTriggerType::Post, 11));
	
	return NewRules;
}

TArray<FRule> ARuleProcessingClassBase::SortBothRules(TArray<FRule> DefaultRules, TArray<FRule> NewRules)
{
	TArray<FRule> ReturnRules;

	ReturnRules.Append(DefaultRules);
	ReturnRules.Append(NewRules);
	ReturnRules = SortRules(ReturnRules);
	
	return ReturnRules;
}

TArray<FRule> ARuleProcessingClassBase::SortRules(TArray<FRule> DefaultRules)
{
	TArray<FRule> ReturnRules = DefaultRules;
	TLinkedList<FRule>* LinkedList = nullptr;
	
	for (int32 RuleIndex = 0; RuleIndex < ReturnRules.Num(); RuleIndex++)
	{
		if (ReturnRules[RuleIndex].TriggingDependencies == TEXT(""))
		{
			TLinkedList<FRule>* NewLinkedList = new TLinkedList<FRule>(ReturnRules[RuleIndex]);
			if (LinkedList == nullptr)
			{
				LinkedList = NewLinkedList;
			}
			else
			{
				LinkedList->LinkHead(NewLinkedList);	
			}
			
		}
	}
	
	PrintLinkedRules(LinkedList);
	return ReturnRules;
}

void ARuleProcessingClassBase::PrintRules(TArray<FRule> DefaultRules)
{
	
}

void ARuleProcessingClassBase::PrintLinkedRules(TLinkedList<FRule>* LinkedList)
{
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		UE_LOG(LogTemp, Warning, TEXT("LinkedRules : %s"), *Rule.RuleName);
	}
}

