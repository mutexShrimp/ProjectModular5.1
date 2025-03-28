// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/RuleProcessingClassBase.h"

#include "Class/ModuleEventCallableInterface.h"
#include "Class/InvokeProcessing/InvokeProcessing_AllModule.h"
#include "Class/InvokeProcessing/InvokeProcessing_AllModule_Once.h"
#include "Class/InvokeProcessing/InvokeProcessing_CurModule.h"
#include "Class/InvokeProcessing/InvokeProcessing_CurModule_Once.h"

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

void ARuleProcessingClassBase::HandleInvocation(UModuleComponent* ModuleComponent, UModuleComponent*& PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules)
{
	// Sort
	TArray<FRule> DefaultRules = InitializeRuleProcessor(ModuleComponent, PreviousComponent, NewModules);

	TArray<FRule> AddedRules = AddRuleProcessor(ModuleComponent, PreviousComponent, NewModules);

	if (!DefaultRules.IsEmpty() || !AddedRules.IsEmpty())
	{
		TArray<FRule> RullRules = SortBothRules(DefaultRules, AddedRules);
	
		for (FRule Rule : RullRules)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rule Processing Function Implemented %s"), *Rule.RuleName);
		}
		
		// Invoke
		for (FRule DefaultRule : RullRules)
		{
			DefaultRule.InvokeProcessingObject->InvokeProcessing(ModuleComponent, PreviousComponent, NewModules, DefaultRule);
		}

		PreviousComponent = ModuleComponent;
	}
	
}

TArray<FRule> ARuleProcessingClassBase::InitializeRuleProcessor_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules)
{
	UE_LOG(LogTemp, Warning, TEXT("InitializeRuleProcessor Function is Invoked"));

	TArray<FRule> NewRules;

	if (!(InvokeProcessingObjects.Contains(UInvokeProcessing_AllModule::StaticClass())))
	{
		InvokeProcessingObjects.Add(UInvokeProcessing_AllModule::StaticClass(), NewObject<UInvokeProcessing_AllModule>(this));	
	}
	if (!(InvokeProcessingObjects.Contains(UInvokeProcessing_CurModule::StaticClass())))
	{
		InvokeProcessingObjects.Add(UInvokeProcessing_CurModule::StaticClass(), NewObject<UInvokeProcessing_CurModule>(this));	
	}
	
	// UInvokeProcessing_AllModule* InvokeProcessingObject_AllModule = NewObject<UInvokeProcessing_AllModule>(this);
	// UInvokeProcessing_CurModule* InvokeProcessingObject_CurrentModule = NewObject<UInvokeProcessing_CurModule>(this);
	
	NewRules.Add(FRule(TEXT("ModuleInitializationBegins_InvokeAll"), *InvokeProcessingObjects.Find(UInvokeProcessing_AllModule::StaticClass())));

	NewRules.Add(FRule(TEXT("ModuleInitializationCompleted_InvokeAll"), *InvokeProcessingObjects.Find(UInvokeProcessing_AllModule::StaticClass())));

	NewRules.Add(FRule(TEXT("InitModule"), *InvokeProcessingObjects.Find(UInvokeProcessing_CurModule::StaticClass()), TEXT("ModuleInitializationBegins_InvokeAll"), ERuleTriggerType::Post, 10));
	
	return NewRules;
}

TArray<FRule> ARuleProcessingClassBase::AddRuleProcessor_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules)
{
	UE_LOG(LogTemp, Warning, TEXT("AddRuleProcessor Function is Invoked"));

	TArray<FRule> NewRules;

	if (!(InvokeProcessingObjects.Contains(UInvokeProcessing_CurModule_Once::StaticClass())))
	{
		InvokeProcessingObjects.Add(UInvokeProcessing_CurModule_Once::StaticClass(), NewObject<UInvokeProcessing_CurModule_Once>(this));	
	}
	if (!(InvokeProcessingObjects.Contains(UInvokeProcessing_AllModule_Once::StaticClass())))
	{
		InvokeProcessingObjects.Add(UInvokeProcessing_AllModule_Once::StaticClass(), NewObject<UInvokeProcessing_AllModule_Once>(this));	
	}
	
	// UInvokeProcessing_CurModule_Once* InvokeProcessingObject_CurrentModule_DoOnce = NewObject<UInvokeProcessing_CurModule_Once>(this);
	// UInvokeProcessing_AllModule_Once* InvokeProcessingObject_AllModule_DoOnce = NewObject<UInvokeProcessing_AllModule_Once>(this);
	
	NewRules.Add(FRule(TEXT("DoOnce_PreInitModule_InvokeAll"), *InvokeProcessingObjects.Find(UInvokeProcessing_AllModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Pre, 10));

	NewRules.Add(FRule(TEXT("DoOnce_PreInitModule"), *InvokeProcessingObjects.Find(UInvokeProcessing_CurModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Pre, 11));

	NewRules.Add(FRule(TEXT("DoOnce_PostInitModule_InvokeAll"), *InvokeProcessingObjects.Find(UInvokeProcessing_AllModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Post, 10));

	NewRules.Add(FRule(TEXT("DoOnce_PostInitModule"), *InvokeProcessingObjects.Find(UInvokeProcessing_CurModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Post, 11));
	
	return NewRules;
}

void ARuleProcessingClassBase::ResetRules()
{
	FinalRules.Empty();
	FinalRuleArr.Empty();
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
	ResetRules();
	
	TArray<FRule> ReturnRules = DefaultRules;
	TLinkedList<FRule>* LinkedList = nullptr;
	TLinkedList<FRule>* LinkedList_Reverse = nullptr;

	// 1 : 初始化无依赖结点
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

	// 2 : 删除无依赖节点缓存
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

	// 3 : 粗略加入依赖节点到对应位置
	for (int32 Index = 0; Index < ReturnRules.Num(); Index++)
	{
		LinkedList = LinkedListToHead(ELinkedListToward::Forward);
		
		for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
		{
			FRule Rule = *It;
			
			if (ReturnRules[Index].TriggingDependencies != TEXT("") && ReturnRules[Index].TriggingDependencies == Rule.RuleName)
			{
				TLinkedList<FRule>* NewLinkedList = new TLinkedList<FRule>(ReturnRules[Index]);
				//TLinkedList<FRule>* NewLinkedList_Reverse = new TLinkedList<FRule>(ReturnRules[Index]);
				switch (ReturnRules[Index].TriggerType)
				{
					case ERuleTriggerType::Pre:
						// (*LinkedList->GetPrevLink())->LinkHead(NewLinkedList);
						// (*LinkedList->GetPrevLink())->GetNextLink()->LinkHead(LinkedList);
					
						//LinkedList->LinkBefore(NewLinkedList);
						{
							TLinkedList<FRule>* PrevLink = GetPrevLink(ELinkedListToward::Forward, LinkedList);
							if (PrevLink != LinkedList && PrevLink != nullptr)
							{
								NewLinkedList->LinkHead(LinkedList);
								PrevLink->LinkHead(NewLinkedList);
							
								//HeadLinkedList = PrevLink;
							
								// TLinkedList<FRule>* PrevLink = *LinkedList->GetPrevLink();
								// PrevLink->LinkHead(NewLinkedList);
								// PrevLink->GetNextLink()->LinkHead(LinkedList);

								//AddReverseLink*
								// TLinkedList<FRule>* Link = GetPrevLink(ELinkedListToward::Backward, LinkedList_Reverse)->GetNextLink();
								// TLinkedList<FRule>* NextLink = Link->GetNextLink();
								// Link->LinkHead(NewLinkedList_Reverse);
								// Link->GetNextLink()->LinkHead(NextLink);
							
							}
							else
							{
								NewLinkedList->LinkHead(LinkedList);
								HeadLinkedList = NewLinkedList;
								//LinkedList = HeadLinkedList;
							
							
								//AddReverseLink*
								// TLinkedList<FRule>* Link = GetPrevLink(ELinkedListToward::Backward, NewLinkedList)->GetNextLink();
								// Link->LinkHead(NewLinkedList_Reverse);
							}
							//(*LinkedList->GetPrevLink())->LinkHead(NewLinkedList);
							// *(*LinkedList->GetPrevLink())->GetNextLink() = *NewLinkedList;
							// *LinkedList->GetPrevLink() = NewLinkedList;
						}
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

							//AddReverseLink*
							// TLinkedList<FRule>* NextRules = LinkedList_Reverse->GetNextLink();
							// LinkedList_Reverse->LinkHead(NewLinkedList_Reverse);
							// LinkedList_Reverse->GetNextLink()->LinkHead(NextRules);
							// HeadLinkedList_Reverse = NewLinkedList_Reverse;
							
						}
						else
						{
							LinkedList->LinkHead(NewLinkedList);

							//AddReverseLink*
							// NewLinkedList_Reverse->LinkHead(LinkedList_Reverse);
							// LinkedList_Reverse = NewLinkedList_Reverse;
							// HeadLinkedList_Reverse = NewLinkedList_Reverse;
						}
						
						
						// *(LinkedList->GetNextLink())->GetPrevLink() = NewLinkedList;;
						// *NewLinkedList->GetNextLink() = *NewLinkedList;
						break;
				}
			}
			if (LinkedList->GetNextLink() != nullptr)
			{
				LinkedList = LinkedList->GetNextLink();
			}
			else
			{
				break;
			}
		}
		//PrintLinkedRules(ELinkedListToward::Forward, LinkedList);
	}
	
	//PrintLinkedRules(ELinkedListToward::Forward, LinkedList);

	// 4 : 根据等级精细排序
	LinkedList = LinkedListToHead(ELinkedListToward::Forward);

	// 转化为数组
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		bool bHasTrigger = false;

		if (Rule.TriggingDependencies == TEXT(""))
		{
			TArray<FRule> NewRules;
			NewRules.Add(Rule);
			FinalRules.Add(NewRules);
		}
		else
		{
			for (int32 Index = 0; Index < FinalRules.Num(); Index++)
			{
				if (FinalRules[Index].Num() > 0)
				{
					if (FinalRules[Index][0].TriggingDependencies == Rule.TriggingDependencies && FinalRules[Index][0].TriggerType == Rule.TriggerType)
					{
						FinalRules[Index].Add(Rule);
						bHasTrigger = true;
					}
				}
			}
			if (!bHasTrigger)
			{
				TArray<FRule> NewRules;
				NewRules.Add(Rule);
				FinalRules.Add(NewRules);
			}
		}
	}

	// 对数组排序
	for (TArray<FRule>& MyRuleArr : FinalRules)
	{
		MyRuleArr.Sort([](const FRule& A, const FRule& B)
		{
			return A.TriggerLevel < B.TriggerLevel;
		});
	}

	//PrintArrayRules(FinalRules);

	// 降维存储
	for (const TArray<FRule>& L_FinalRuleArr : FinalRules)
	{
		for (FRule FinalRule : L_FinalRuleArr)
		{
			FinalRuleArr.Add(FinalRule);
		}
	}
	//PrintRules(FinalRuleArr);


	
	
	// LinkedList = LinkedListToHead(ELinkedListToward::Forward);
	// TLinkedList<FRule>* L_HeadLinkedList = LinkedListToHead(ELinkedListToward::Forward);
	//
	// // 加入数组
	// TArray<TArray<FRule>> RuleArray;
	// for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	// {
	// 	//FRule Rule = *It;
	//
	// 	if (L_HeadLinkedList->GetNextLink() != nullptr)
	// 	{
	// 		if ((*L_HeadLinkedList)->TriggingDependencies == (*L_HeadLinkedList->GetNextLink())->TriggingDependencies)
	// 		{
	// 			FRule Rule(
	// 			(*L_HeadLinkedList)->RuleName,
	// 			(*L_HeadLinkedList)->InvokeProcessingObject,
	// 			(*L_HeadLinkedList)->TriggingDependencies,
	// 			(*L_HeadLinkedList)->TriggerType,
	// 			(*L_HeadLinkedList)->TriggerLevel);
	//
	// 			FRule NextRule(
	// 			(*L_HeadLinkedList->GetNextLink())->RuleName,
	// 			(*L_HeadLinkedList->GetNextLink())->InvokeProcessingObject,
	// 			(*L_HeadLinkedList->GetNextLink())->TriggingDependencies,
	// 			(*L_HeadLinkedList->GetNextLink())->TriggerType,
	// 			(*L_HeadLinkedList->GetNextLink())->TriggerLevel);
	//
	// 			if (RuleArray.Num() == 0)
	// 			{
	// 				TArray<FRule> NewRuleArray;
	// 				NewRuleArray.Add(Rule);
	// 				NewRuleArray.Add(NextRule);
	// 				RuleArray.Add(NewRuleArray);
	// 				
	// 			}
	// 			else
	// 			{
	// 				RuleArray[RuleArray.Num() - 1].Add(Rule);
	// 				RuleArray[RuleArray.Num() - 1].Add(NextRule);
	// 			}
	//
	// 			L_HeadLinkedList = L_HeadLinkedList->GetNextLink();
	// 		}
	// 		else
	// 		{
	// 			if (RuleArray.Num() > 0)
	// 			{
	// 				TArray<FRule> NewRuleArray;
	// 				RuleArray.Add(NewRuleArray);
	// 			}
	// 			L_HeadLinkedList = L_HeadLinkedList->GetNextLink();
	// 		}
	// 	}
	// 	
	// }
	//
	// PrintArrayRules(RuleArray);
	//
	// // 数组排序
	//
	//
	// // 链表链接
	// LinkedList = LinkedListToHead(ELinkedListToward::Forward);
	// TLinkedList<FRule>* NewLinkedList2;
	//
	// {
	// 	FRule Rule(
	// 		(*LinkedList)->RuleName,
	// 		(*LinkedList)->InvokeProcessingObject,
	// 		(*LinkedList)->TriggingDependencies,
	// 		(*LinkedList)->TriggerType,
	// 		(*LinkedList)->TriggerLevel);
	//
	// 	CacheLinkList = new TLinkedList<FRule>(Rule);
	// 	NewLinkedList2 = CacheLinkList;
	// }
	//
	// // 初始移动
	// while (LinkedList->GetNextLink() != nullptr && (*LinkedList->GetNextLink())->TriggingDependencies != (*LinkedList)->TriggingDependencies)
	// {
	// 	LinkedList = LinkedList->GetNextLink();
	//
	// 	
	// 	FRule Rule(
	// 	(*LinkedList)->RuleName,
	// 	(*LinkedList)->InvokeProcessingObject,
	// 	(*LinkedList)->TriggingDependencies,
	// 	(*LinkedList)->TriggerType,
	// 	(*LinkedList)->TriggerLevel);
	// 	
	// 	TLinkedList<FRule>* L_NewLinkedList = new TLinkedList<FRule>(Rule);
	// 	NewLinkedList2->LinkHead(L_NewLinkedList);
	// 	
	// }
	// LinkedList = GetPrevLink(ELinkedListToward::Forward, LinkedList);
	// NewLinkedList2 = GetNewPrevLink(NewLinkedList2, CacheLinkList);
	//
	// L_HeadLinkedList = LinkedList;
	//
	// if (RuleArray.Num() > 0)
	// {
	// 	for (int32 i = 0; i < RuleArray.Num(); i++)
	// 	{
	// 		for (int32 j = 0; j < RuleArray[i].Num(); j++)
	// 		{
	// 			if ((*L_HeadLinkedList->GetNextLink())->TriggingDependencies == RuleArray[i][j].TriggingDependencies)
	// 			{
	// 				FRule Rule(
	// 					RuleArray[i][j].RuleName,
	// 					RuleArray[i][j].InvokeProcessingObject,
	// 					RuleArray[i][j].TriggingDependencies,
	// 					RuleArray[i][j].TriggerType,
	// 					RuleArray[i][j].TriggerLevel
	// 					);
	// 				TLinkedList<FRule>* L_NewLinkedList2 = new TLinkedList<FRule>(Rule);
	// 				NewLinkedList2->LinkHead(L_NewLinkedList2);
	// 				
	// 				L_HeadLinkedList = L_HeadLinkedList->GetNextLink();
	// 			}
	// 		}
	//
	// 		if (L_HeadLinkedList->GetNextLink() != nullptr)
	// 		{
	// 			//L_HeadLinkedList = L_HeadLinkedList->GetNextLink();
	//
	// 			
	// 		}
	// 		
	// 	}
	// }
	// LinkedList = L_HeadLinkedList;



	
	//PrintLinkedRules(ELinkedListToward::Forward, LinkedList);
	
	return FinalRuleArr;
}

void ARuleProcessingClassBase::PrintArrayRules(TArray<TArray<FRule>> DefaultArrayRules)
{
	for (TArray<FRule> RuleArray : DefaultArrayRules)
	{
		PrintRules(RuleArray);
	}
}

void ARuleProcessingClassBase::PrintRules(TArray<FRule> DefaultRules)
{
	for ( FRule Rule : DefaultRules)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rules : %s"), *Rule.RuleName);
	}
}

void ARuleProcessingClassBase::PrintLinkedRules(ELinkedListToward LinkedListToward, TLinkedList<FRule>* LinkedList)
{
	LinkedList = LinkedListToHead(LinkedListToward);
	
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		UE_LOG(LogTemp, Warning, TEXT("Toward %s, LinkedRules : %s"), LinkedListToward == ELinkedListToward::Forward ? TEXT("Forward") : TEXT("Backward"), *Rule.RuleName);
	}
	UE_LOG(LogTemp, Display, TEXT("----------\n"));
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
			//TLinkedList<FRule>* BackLinkedList = GetPrevLink(ELinkedListToward::Backward, LinkedList);
			
			for (TLinkedList<FRule>::TIterator It(HeadLinkedList); It; It.Next())
			{
				FRule Rule = *It;

				if (__LinkedList->GetNextLink() != nullptr)
				{
					if ((*__LinkedList->GetNextLink())->RuleName == (*LinkedList)->RuleName)
					{
						return __LinkedList;
					}
					
					__LinkedList = __LinkedList->GetNextLink();	
				}
				else
				{
					return nullptr;
				}
			}
		}
		break;
	case ELinkedListToward::Backward:
		TLinkedList<FRule>* LinkedList_Reverse = HeadLinkedList_Reverse;
		for (TLinkedList<FRule>::TIterator It(HeadLinkedList_Reverse); It; It.Next())
		{
			FRule Rule = *It;
			if ((*LinkedList->GetNextLink())->RuleName == Rule.RuleName)
			{
				return LinkedList_Reverse;
			}
		
			LinkedList_Reverse = LinkedList_Reverse->GetNextLink();
		}
		break;
	}
	return NULL;
}

TLinkedList<FRule>* ARuleProcessingClassBase::GetNewPrevLink(TLinkedList<FRule>* LinkedList,
	TLinkedList<FRule>* __HeadLinkedList)
{
	TLinkedList<FRule>* __LinkedList = __HeadLinkedList;
			
	for (TLinkedList<FRule>::TIterator It(__HeadLinkedList); It; It.Next())
	{
		FRule Rule = *It;

		if (__LinkedList->GetNextLink() != nullptr)
		{
			if ((*__LinkedList->GetNextLink())->RuleName == (*LinkedList)->RuleName)
			{
				return __LinkedList;
			}
					
			__LinkedList = __LinkedList->GetNextLink();	
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}
