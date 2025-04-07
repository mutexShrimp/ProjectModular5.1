// Copyright Shrimp Chen 2025 All Rights Reserved.


#include "Class/RuleProcessingClassBase.h"
#include "Engine/Engine.h"
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

void ARuleProcessingClassBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	InvokeProcessingObjects.Empty();
	GEngine->ForceGarbageCollection(true);
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
	HeadLinkedList = nullptr;
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

	// 加入树状数组中
	TArray<FRulesOrderProcessing> RulesOrderProcessingArr;
	for (TLinkedList<FRule>::TIterator It(LinkedList); It; It.Next())
	{
		FRule Rule = *It;
		FRulesOrderProcessing RulesOrderProcessing;
		RulesOrderProcessing.SetRule(Rule);

		for (TLinkedList<FRule>::TIterator L_It(LinkedList); L_It; L_It.Next())
		{
			FRule L_Rule = *L_It;

			if (L_Rule.RuleName != Rule.RuleName)
			{
				if (L_Rule.TriggingDependencies == Rule.RuleName)
				{
					switch (L_Rule.TriggerType) {
						case ERuleTriggerType::Pre:
							RulesOrderProcessing.AddPreRule(L_Rule);
							break;
						case ERuleTriggerType::Post:
							RulesOrderProcessing.AddPostRule(L_Rule);
							break;
						default: ;
					}
				}
			}
		}
		
		RulesOrderProcessingArr.Add(RulesOrderProcessing);
	}

	// 遍历排序
	for (FRulesOrderProcessing& OrderProcessing : RulesOrderProcessingArr)
	{
		if (OrderProcessing.GetPreRules().Num() > 1)
		{
			OrderProcessing.GetPreRules().Sort([](const FRulesOrderProcessing& A, const FRulesOrderProcessing& B)
			{
				return A.GetRule().TriggerLevel < B.GetRule().TriggerLevel;
			});	
		}

		if (OrderProcessing.GetPostRules().Num() > 1)
		{
			OrderProcessing.GetPostRules().Sort([](const FRulesOrderProcessing& A, const FRulesOrderProcessing& B)
			{
				return A.GetRule().TriggerLevel < B.GetRule().TriggerLevel;
			});	
		}
	}
	
	// 遍历获取
	TArray<int32> IgnoreIndexes;
	
	for (FRulesOrderProcessing OrderProcessing : RulesOrderProcessingArr)
	{
		if (!(IgnoreIndexes.Contains(GetLinkedRuleIndex(OrderProcessing.GetRule().RuleName))))
		{
			if (OrderProcessing.GetPreRules().Num() > 0 || OrderProcessing.GetPostRules().Num() > 0 || OrderProcessing.GetRule().TriggingDependencies == TEXT(""))
			{
				FinalRuleArr.Append(PreorderTraversalRules(OrderProcessing, RulesOrderProcessingArr, IgnoreIndexes));
			}
		}
	}
	
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

int32 ARuleProcessingClassBase::GetIndexByRuleName(FRulesOrderProcessing RulesOrderProcessing, TArray<FRulesOrderProcessing> RulesOrderProcessingArr,
	TArray<int32> IgnoreIndexes)
{
	int32 Index = 0;
	for (FRulesOrderProcessing L_RulesOrderProcessing : RulesOrderProcessingArr)
	{
		if (L_RulesOrderProcessing.GetRuleName() == RulesOrderProcessing.GetRuleName())
		{
			if (IgnoreIndexes.Contains(Index))
			{
				return -1;
			}
			return Index;
		}
		Index++;
	}
	return -1;
}

FRulesOrderProcessing ARuleProcessingClassBase::GetRuleByIndex(int32 Index, TArray<FRulesOrderProcessing> RulesOrderProcessingArr)
{
	int32 L_Index = 0;
	for (FRulesOrderProcessing L_RulesOrderProcessing : RulesOrderProcessingArr)
	{
		if (Index == L_Index)
		{
			return L_RulesOrderProcessing;
		}
		L_Index++;
	}
	return FRulesOrderProcessing();
}

TArray<FRule> ARuleProcessingClassBase::PreorderTraversalRules(FRulesOrderProcessing RulesOrderProcessing,
                                                     TArray<FRulesOrderProcessing> RulesOrderProcessingArr, TArray<int32>& IgnoreIndexes)
{
	TArray<FRule> Rules;
	// 前向遍历
	for (FRulesOrderProcessing PreRule : RulesOrderProcessing.GetPreRules())
	{
		int32 IndexByRuleName = GetIndexByRuleName(PreRule, RulesOrderProcessingArr, IgnoreIndexes);
		if (IndexByRuleName == -1)
		{
			// 无需继续寻找遍历 进行后续迭代
			break;
		}
		else
		{
			// 需要继续寻找遍历
			// 获取前序对象，进入对象继续遍历，当前前序对象加入返回节点，加入忽视索引
			FRulesOrderProcessing L_RulesOrderProcessing = GetRuleByIndex(IndexByRuleName, RulesOrderProcessingArr);
			TArray<FRule> TraversalRules = PreorderTraversalRules(L_RulesOrderProcessing, RulesOrderProcessingArr, IgnoreIndexes);
			Rules.Append(TraversalRules);
			IgnoreIndexes.AddUnique(IndexByRuleName);
			int32 L_IndexByRuleName = GetIndexByRuleName(L_RulesOrderProcessing, RulesOrderProcessingArr, IgnoreIndexes);
			if (L_IndexByRuleName != -1)
			{
				Rules.Add(L_RulesOrderProcessing.GetRule());	
			}
		}
	}

	// 中序遍历
	int32 IndexByRuleName = GetIndexByRuleName(RulesOrderProcessing, RulesOrderProcessingArr, IgnoreIndexes);
	if (IndexByRuleName != -1)
	{
		Rules.Add(RulesOrderProcessing.GetRule());
		IgnoreIndexes.AddUnique(IndexByRuleName);
	}
	
	// 后续遍历
	for (FRulesOrderProcessing PostRule : RulesOrderProcessing.GetPostRules())
	{
		int32 L_IndexByRuleName = GetIndexByRuleName(PostRule, RulesOrderProcessingArr, IgnoreIndexes);
		if (L_IndexByRuleName == -1)
		{
			// 无需继续寻找遍历 进行后续迭代
			break;
		}
		else
		{
			// 需要继续寻找遍历
			// 获取前序对象，进入对象继续遍历，当前前序对象加入返回节点，加入忽视索引
			FRulesOrderProcessing L_RulesOrderProcessing = GetRuleByIndex(L_IndexByRuleName, RulesOrderProcessingArr);
			TArray<FRule> TraversalRules = PreorderTraversalRules(L_RulesOrderProcessing, RulesOrderProcessingArr, IgnoreIndexes);
			// Rules.Add(L_RulesOrderProcessing.GetRule());
			Rules.Append(TraversalRules);
			// IgnoreIndexes.Add(L_IndexByRuleName);
		}
	}
	
	return Rules;
}

