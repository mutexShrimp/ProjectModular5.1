// Copyright Shrimp Chen 2025 All Rights Reserved.


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

bool ARuleProcessingClassBase::HandleInvocation(UModuleComponent* ModuleComponent, UModuleComponent*& PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules)
{
	InvokeProcessingObjects.Empty();
	
	// Sort
	TArray<FRule> DefaultRules = InitializeRuleProcessor(ModuleComponent, PreviousComponent, NewModules);

	TArray<FRule> AddedRules = AddRuleProcessor(ModuleComponent, PreviousComponent, NewModules);

	if (!DefaultRules.IsEmpty() || !AddedRules.IsEmpty())
	{
		TArray<FRule> RullRules = SortBothRules(DefaultRules, AddedRules);
		
		// Invoke
		for (FRule DefaultRule : RullRules)
		{
			DefaultRule.InvokeProcessingObject->InvokeProcessing(ModuleComponent, PreviousComponent, NewModules, DefaultRule);
		}

		PreviousComponent = ModuleComponent;

		return true;
	}

	return false;
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
	
	NewRules.Add(FRule(TEXT("DoOnce_PreInitModule_InvokeAll"), *InvokeProcessingObjects.Find(UInvokeProcessing_AllModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Pre, 10));

	NewRules.Add(FRule(TEXT("DoOnce_PreInitModule"), *InvokeProcessingObjects.Find(UInvokeProcessing_CurModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Pre, 11));

	NewRules.Add(FRule(TEXT("DoOnce_PostInitModule_InvokeAll"), *InvokeProcessingObjects.Find(UInvokeProcessing_AllModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Post, 10));

	NewRules.Add(FRule(TEXT("DoOnce_PostInitModule"), *InvokeProcessingObjects.Find(UInvokeProcessing_CurModule_Once::StaticClass()), TEXT("InitModule"), ERuleTriggerType::Post, 11));
	
	return NewRules;
}

void ARuleProcessingClassBase::ResetRules()
{
	FinalRulesTuple.Empty();
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

	// 1 : 初始化无依赖结点
	for (int32 RuleIndex = 0; RuleIndex < ReturnRules.Num(); RuleIndex++)
	{
		if (ReturnRules[RuleIndex].TriggingDependencies == TEXT(""))
		{
			TLinkedList<FRule>* NewLinkedList = new TLinkedList<FRule>(ReturnRules[RuleIndex]);
			if (LinkedList == nullptr)
			{
				LinkedList = NewLinkedList;
				HeadLinkedList = NewLinkedList;
			}
			else
			{
				LinkedList->LinkHead(NewLinkedList);
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
		LinkedList = LinkedListToHead();
		
		for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
		{
			FRule Rule = *It;
			
			if (ReturnRules[Index].TriggingDependencies != TEXT("") && ReturnRules[Index].TriggingDependencies == Rule.RuleName)
			{
				TLinkedList<FRule>* NewLinkedList = new TLinkedList<FRule>(ReturnRules[Index]);
				
				switch (ReturnRules[Index].TriggerType)
				{
					case ERuleTriggerType::Pre:
						{
							TLinkedList<FRule>* PrevLink = GetPrevLink(LinkedList);
							if (PrevLink != LinkedList && PrevLink != nullptr)
							{
								NewLinkedList->LinkHead(LinkedList);
								PrevLink->LinkHead(NewLinkedList);
							
							}
							else
							{
								NewLinkedList->LinkHead(LinkedList);
								HeadLinkedList = NewLinkedList;
								
							}
						}
						break;
					case ERuleTriggerType::Post:
						if (LinkedList->GetNextLink() != nullptr)
						{
							TLinkedList<FRule>* NextLink = LinkedList->GetNextLink();
							LinkedList->LinkHead(NewLinkedList);
							LinkedList->GetNextLink()->LinkHead(NextLink);
							
						}
						else
						{
							LinkedList->LinkHead(NewLinkedList);

						}
					
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
	}

	// 打印链表最终结构
	PrintLinkedRules(LinkedList);

	// 4 : 根据等级精细排序
	LinkedList = LinkedListToHead();

	// 转化为数组
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		bool bHasTrigger = false;

		if (Rule.TriggingDependencies == TEXT(""))
		{
			int32 LinkedRuleIndex = GetLinkedRuleIndex(Rule.RuleName);
			TArray<TTuple<int32, FRule>> NewRulesTuple;
			NewRulesTuple.Add(TTuple<int32, FRule>(LinkedRuleIndex, Rule));
			FinalRulesTuple.Add(NewRulesTuple);
		}
		else
		{
			for (int32 Index = 0; Index < FinalRulesTuple.Num(); Index++)
			{
				if (FinalRulesTuple[Index].Num() > 0)
				{
					if (FinalRulesTuple[Index][0].Get<1>().TriggingDependencies == Rule.TriggingDependencies && FinalRulesTuple[Index][0].Get<1>().TriggerType == Rule.TriggerType)
					{
						int32 LinkedRuleIndex = GetLinkedRuleIndex(Rule.RuleName);
						FinalRulesTuple[Index].Add(TTuple<int32, FRule>(LinkedRuleIndex, Rule));
						bHasTrigger = true;
					}
				}
			}
			if (!bHasTrigger)
			{
				int32 LinkedRuleIndex = GetLinkedRuleIndex(Rule.RuleName);
				TArray<TTuple<int32, FRule>> NewRulesTuple;
				NewRulesTuple.Add(TTuple<int32, FRule>(LinkedRuleIndex, Rule));
				FinalRulesTuple.Add(NewRulesTuple);
			}
		}
	}

	for (TArray<TTuple<int32, FRule>>& MyRuleArr : FinalRulesTuple)
	{
		MyRuleArr.Sort([](const TTuple<int32, FRule>& A, const TTuple<int32, FRule>& B)
		{
			return A.Get<1>().TriggerLevel < B.Get<1>().TriggerLevel;
		});
	}

	TArray<int32> IgnoreIndexes;
	int32 NowMaxIndex = -1;
	// 最终排序
	for (int32 i = 0; i < FinalRulesTuple.Num(); i++)
	{
		if (!(FinalRulesTuple[i].IsEmpty()))
		{
			// 数组内仅一位数据
			if (FinalRulesTuple[i].Num() == 1)
			{
				if (!(IgnoreIndexes.Contains(FinalRulesTuple[i][0].Get<0>())))
				{
					FinalRuleArr.Add(FinalRulesTuple[i][0].Get<1>());
					NowMaxIndex = (FinalRulesTuple[i][0].Get<0>() > NowMaxIndex ? FinalRulesTuple[i][0].Get<0>() : NowMaxIndex);
				}
			}
			else
			{
				// 数组内多位数据
				for (int32 j = 0; j < FinalRulesTuple[i].Num(); j++)
				{
					if (!(IgnoreIndexes.Contains(FinalRulesTuple[i][j].Get<0>())))
					{
						bool bNextIsValidIndex = FinalRulesTuple.IsValidIndex(i + 1);

						int32 NextIndex = -1;
						if (bNextIsValidIndex)
						{
							NextIndex = FinalRulesTuple[i + 1][0].Get<0>();
						}
						
						// 统计最大索引与最小索引
						int32 L_NowMaxIndex = -1;
						int32 L_NowMinIndex = 99999;
						for (int32 k = 0; k < FinalRulesTuple[i].Num(); k++)
						{
							L_NowMaxIndex = (FinalRulesTuple[i][k].Get<0>() > L_NowMaxIndex ? FinalRulesTuple[i][k].Get<0>() : L_NowMaxIndex);
							L_NowMinIndex = (FinalRulesTuple[i][k].Get<0>() < L_NowMinIndex ? FinalRulesTuple[i][k].Get<0>() : L_NowMinIndex);
						}
						
						if (j == 0)
						{
							// 首条数据
							FinalRuleArr.Add(FinalRulesTuple[i][j].Get<1>());
							NowMaxIndex = (FinalRulesTuple[i][j].Get<0>() > NowMaxIndex ? FinalRulesTuple[i][j].Get<0>() : NowMaxIndex);
						}
						else
						{
							if (bNextIsValidIndex)
							{
								if (L_NowMinIndex < NextIndex && L_NowMaxIndex < NextIndex)
								{
									// 如果最大最小索引都小于下个索引
									FinalRuleArr.Add(FinalRulesTuple[i][j].Get<1>());
								}
								else
								{
									// 后续有节点需插入当前数组
									for (int32 k = i + 1; k < FinalRulesTuple.Num(); k++)
									{
										for (int32 l = 0; l < FinalRulesTuple[k].Num(); l++)
										{
											if (FinalRulesTuple[k][l].Get<0>() < L_NowMaxIndex)
											{
												if (FinalRulesTuple[k][l].Get<0>() == NowMaxIndex + 1)
												{
													FinalRuleArr.Add(FinalRulesTuple[k][l].Get<1>());
													NowMaxIndex = (FinalRulesTuple[k][l].Get<0>() > NowMaxIndex ? FinalRulesTuple[k][l].Get<0>() : NowMaxIndex);
													IgnoreIndexes.Add(FinalRulesTuple[k][l].Get<0>());
												}
											}
										}
									}
									// 添加完成后续的前置节点后加入当前节点
									FinalRuleArr.Add(FinalRulesTuple[i][j].Get<1>());
								}	
							}
							else
							{
								// 如果没有后续节点则直接加入
								FinalRuleArr.Add(FinalRulesTuple[i][j].Get<1>());	
							}
						}
					}
				}
			}	
		}
	}

	// 打印数组最终结构
	PrintRules(FinalRuleArr);
	
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
	UE_LOG(LogTemp, Display, TEXT("----- Start Array Print -----\n"));
	for ( FRule Rule : DefaultRules)
	{
		UE_LOG(LogTemp, Warning, TEXT("Array Rules : %s"), *Rule.RuleName);
	}
	UE_LOG(LogTemp, Display, TEXT("----- End Array Print -----\n"));
}

void ARuleProcessingClassBase::PrintLinkedRules(TLinkedList<FRule>* LinkedList)
{
	LinkedList = LinkedListToHead();

	UE_LOG(LogTemp, Display, TEXT("----- Start Linked List Print -----\n"));
	
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		UE_LOG(LogTemp, Warning, TEXT("Linked List Rules : %s"), *Rule.RuleName);
	}
	UE_LOG(LogTemp, Display, TEXT("----- End Linked List Print -----\n"));
}

int32 ARuleProcessingClassBase::GetLinkedRuleIndex(const FString& RuleName)
{
	TLinkedList<FRule>* LinkedList = LinkedListToHead();
	int32 Index = 0;
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		if (Rule.RuleName == RuleName)
		{
			return Index;
		}
		Index++;
	}
	return -1;
}

TLinkedList<FRule>* ARuleProcessingClassBase::LinkedListToHead()
{
	return HeadLinkedList;
}

TLinkedList<FRule>* ARuleProcessingClassBase::GetPrevLink(TLinkedList<FRule>* LinkedList)
{
	TLinkedList<FRule>* __LinkedList = LinkedListToHead();
	
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
	
	return nullptr;
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
