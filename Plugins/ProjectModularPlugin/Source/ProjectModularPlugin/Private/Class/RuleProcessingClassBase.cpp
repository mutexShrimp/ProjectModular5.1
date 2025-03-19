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

	if (!DefaultRules.IsEmpty() || !AddedRules.IsEmpty())
	{
		SortBothRules(DefaultRules, AddedRules);
	
		for (FRule Rule : DefaultRules)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rule Processing Function Implemented %s"), *Rule.RuleName);
		}
	
		// Invoke
		// for (FRule DefaultRule : DefaultRules)
		// {
		// 	DefaultRule.InvokeProcessingObject->InvokeProcessing(ModuleComponent);
		// }	
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
	
	NewRules.Add(FRule(TEXT("InitModule"), InvokeProcessingObject_CurrentModule, TEXT("ModuleInitializationBegins_InvokeAll"), ERuleTriggerType::Post, 10));

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
	TLinkedList<FRule>* LinkedList_Reverse = nullptr;
	
	for (int32 RuleIndex = 0; RuleIndex < ReturnRules.Num(); RuleIndex++)
	{
		if (ReturnRules[RuleIndex].TriggingDependencies == TEXT(""))
		{
			TLinkedList<FRule>* NewLinkedList = new TLinkedList<FRule>(ReturnRules[RuleIndex]);
			TLinkedList<FRule>* NewLinkedList_Reverse = new TLinkedList<FRule>(ReturnRules[RuleIndex]);
			if (LinkedList == nullptr)
			{
				LinkedList = NewLinkedList;
				HeadLinkedList = NewLinkedList;
				LinkedList_Reverse = NewLinkedList_Reverse;
				HeadLinkedList_Reverse = NewLinkedList_Reverse;
				if (HeadLinkedList_Reverse)
				{
					
				}
			}
			else
			{
				LinkedList->LinkHead(NewLinkedList);
				
				NewLinkedList_Reverse->LinkHead(LinkedList_Reverse);
				LinkedList_Reverse = NewLinkedList_Reverse;
				HeadLinkedList_Reverse = NewLinkedList_Reverse;
			}
		}
	}

	bool bRemoved = true;
	while (bRemoved)
	{
		bRemoved = false;
		for (int32 RemoveIndex = 0; RemoveIndex < ReturnRules.Num(); RemoveIndex++)
		{
			if (ReturnRules[RemoveIndex].TriggingDependencies == TEXT(""))
			{
				ReturnRules.RemoveAt(RemoveIndex);
				bRemoved = true;
				break;
			}
		}
	}
	
	for (int32 Index = 0; Index < ReturnRules.Num(); Index++)
	{
		LinkedList = LinkedListToHead(ELinkedListToward::Forward);
		
		for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
		{
			FRule Rule = *It;
			
			if (ReturnRules[Index].TriggingDependencies != TEXT("") && ReturnRules[Index].TriggingDependencies == Rule.RuleName)
			{
				TLinkedList<FRule>* NewLinkedList = new TLinkedList<FRule>(ReturnRules[Index]);
				TLinkedList<FRule>* NewLinkedList_Reverse = new TLinkedList<FRule>(ReturnRules[Index]);
				switch (ReturnRules[Index].TriggerType)
				{
					case ERuleTriggerType::Pre:
						// (*LinkedList->GetPrevLink())->LinkHead(NewLinkedList);
						// (*LinkedList->GetPrevLink())->GetNextLink()->LinkHead(LinkedList);
					
						//LinkedList->LinkBefore(NewLinkedList);
						if (GetPrevLink(ELinkedListToward::Forward, LinkedList) != LinkedList)
						{
							TLinkedList<FRule>* PrevLink = GetPrevLink(ELinkedListToward::Forward, LinkedList);
							NewLinkedList->LinkHead(LinkedList);
							PrevLink->LinkHead(NewLinkedList);
							HeadLinkedList = PrevLink;
							// TLinkedList<FRule>* PrevLink = *LinkedList->GetPrevLink();
							// PrevLink->LinkHead(NewLinkedList);
							// PrevLink->GetNextLink()->LinkHead(LinkedList);

							//AddReverseLink
							TLinkedList<FRule>* Link = GetPrevLink(ELinkedListToward::Backward, LinkedList_Reverse)->GetNextLink();
							TLinkedList<FRule>* NextLink = Link->GetNextLink();
							Link->LinkHead(NewLinkedList_Reverse);
							Link->GetNextLink()->LinkHead(NextLink);
							
						}
						else
						{
							NewLinkedList->LinkHead(LinkedList);
							HeadLinkedList = NewLinkedList;
							
							//AddReverseLink
							TLinkedList<FRule>* Link = GetPrevLink(ELinkedListToward::Backward, NewLinkedList)->GetNextLink();
							Link->LinkHead(NewLinkedList_Reverse);
						}
						//(*LinkedList->GetPrevLink())->LinkHead(NewLinkedList);
						// *(*LinkedList->GetPrevLink())->GetNextLink() = *NewLinkedList;
						// *LinkedList->GetPrevLink() = NewLinkedList;
						break;
					case ERuleTriggerType::Post:
						// *(LinkedList->GetNextLink())->GetPrevLink() = NewLinkedList;;
						// *NewLinkedList->GetNextLink() = *NewLinkedList;
					
						//LinkedList->LinkAfter(NewLinkedList);
						if (LinkedList->GetNextLink() != nullptr)
						{
							TLinkedList<FRule>* NextLink = LinkedList->GetNextLink();
							LinkedList->LinkHead(NewLinkedList);
							LinkedList->GetNextLink()->LinkHead(NextLink);

							//AddReverseLink
							TLinkedList<FRule>* NextRules = LinkedList_Reverse->GetNextLink();
							LinkedList_Reverse->LinkHead(NewLinkedList_Reverse);
							LinkedList_Reverse->GetNextLink()->LinkHead(NextRules);
							HeadLinkedList_Reverse = NewLinkedList_Reverse;
							
						}
						else
						{
							LinkedList->LinkHead(NewLinkedList);

							//AddReverseLink
							NewLinkedList_Reverse->LinkHead(LinkedList_Reverse);
							LinkedList_Reverse = NewLinkedList_Reverse;
							HeadLinkedList_Reverse = NewLinkedList_Reverse;
						}
						
						
						// *(LinkedList->GetNextLink())->GetPrevLink() = NewLinkedList;;
						// *NewLinkedList->GetNextLink() = *NewLinkedList;
						break;
				}
			}
			if (LinkedList->GetNextLink())
			{
				LinkedList = LinkedList->GetNextLink();
			}
		}
	}
	//LinkedList = HeadLinkedList;
	PrintLinkedRules(ELinkedListToward::Forward, LinkedList);
	PrintLinkedRules(ELinkedListToward::Backward, LinkedList_Reverse);
	
	return ReturnRules;
}

void ARuleProcessingClassBase::PrintRules(TArray<FRule> DefaultRules)
{
	
}

void ARuleProcessingClassBase::PrintLinkedRules(ELinkedListToward LinkedListToward, TLinkedList<FRule>* LinkedList)
{
	LinkedList = LinkedListToHead(LinkedListToward);
	
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		UE_LOG(LogTemp, Warning, TEXT("Toward %s, LinkedRules : %s"), LinkedListToward == ELinkedListToward::Forward ? TEXT("Forward") : TEXT("Backward"), *Rule.RuleName);
	}
}

TLinkedList<FRule>* ARuleProcessingClassBase::LinkedListToHead(ELinkedListToward LinkedListToward)
{
	switch (LinkedListToward) {
	case ELinkedListToward::Forward:
		return HeadLinkedList;
	case ELinkedListToward::Backward:
		return HeadLinkedList_Reverse;
	}
	return NULL;
	// for (;;)
	// {
	// 	if (*LinkedList->GetPrevLink() && *LinkedList->GetPrevLink() != LinkedList)
	// 	{
	// 		LinkedList = *LinkedList->GetPrevLink();	
	// 	}
	// 	break;
	// }
}

TLinkedList<FRule>* ARuleProcessingClassBase::GetPrevLink(ELinkedListToward LinkedListToward, TLinkedList<FRule>* LinkedList)
{
	switch (LinkedListToward) {
	case ELinkedListToward::Forward:
		{
			TLinkedList<FRule>* __LinkedList = HeadLinkedList;
			TLinkedList<FRule>* BackLinkedList = GetPrevLink(ELinkedListToward::Backward, LinkedList);
			for (TLinkedList<FRule>::TIterator It(HeadLinkedList); It; It.Next())
			{
				FRule Rule = *It;
				if ((*BackLinkedList)->RuleName == Rule.RuleName)
				{
					return __LinkedList;
				}
		
				__LinkedList = __LinkedList->GetNextLink();
			}
		}
		break;
	case ELinkedListToward::Backward:
		TLinkedList<FRule>* LinkedList_Reverse = HeadLinkedList_Reverse;
		for (TLinkedList<FRule>::TIterator It(HeadLinkedList_Reverse); It; It.Next())
		{
			FRule Rule = *It;
			if ((*LinkedList)->RuleName == Rule.RuleName)
			{
				return LinkedList_Reverse->GetNextLink();
			}
		
			LinkedList_Reverse = LinkedList_Reverse->GetNextLink();
		}
		break;
	}
	return NULL;
}