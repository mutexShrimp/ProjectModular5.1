// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/RuleProcessingClassBase.h"

#include "Class/ModuleClassInterface.h"
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
	UFunction* Function = this->FindFunction(TEXT("MyBlueprintFunction"));
	this->ProcessEvent(Function, nullptr);
	
	// TArray<FRule> DefaultRules;
	// TArray<FRule> Rules1 = InitializeRuleProcessor();
	// for (FRule Rule : Rules1)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Rule Processing Function Implemented %s"), *Rule.RuleName);
	// }


	
	// bool bIsBlueprintImplemented = UBlueprintModuleLibrary::IsBlueprintFunctionImplemented(this, TEXT("InitializeRuleProcessor"));
	//
	// if (bIsBlueprintImplemented)
	// {
	// 	UFunction* Function = this->FindFunction("InitializeRuleProcessor");
	// 	if (Function)
	// 	{
	// 		// 分配内存用于存储参数和返回值
	// 		uint8* Params = (uint8*)FMemory_Alloca(Function->ParmsSize);
	// 		FMemory::Memzero(Params, Function->ParmsSize);
	// 		// 调用函数
	// 		this->ProcessEvent(Function, Params);
	// 		// 获取返回值（假设返回值是int32类型）
	// 		TArray<FRule>* ReturnValue = (TArray<FRule>*)(Params + Function->ReturnValueOffset);
	// 	}
	// }
	// else
	// {
	// 	InitializeRuleProcessor_Implementation();
	// }
	//
	// UE_LOG(LogTemp, Warning, TEXT("Rule Processing Function Implemented %d"), bIsBlueprintImplemented);
	
}

TArray<FRule> ARuleProcessingClassBase::InitializeRuleProcessor_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default InitializeRuleProcessor_Implementation of MyFunction"));

	TArray<FRule> NewRules;
	
	NewRules.Add(FRule(TEXT("PreInitialEvent_CallAll")));

	NewRules.Add(FRule(TEXT("PostInitialEvent_CallAll")));

	return NewRules;
}

// void ARuleProcessingClassBase::InitializeRuleProcessor_Implementation(TArray<FRule>& NewRules)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Default InitializeRuleProcessor_Implementation of MyFunction"));
// 	
// 	NewRules.Add(FRule(TEXT("PreInitialEvent_CallAll")));
//
// 	NewRules.Add(FRule(TEXT("PostInitialEvent_CallAll")));
// }

void ARuleProcessingClassBase::AddRuleProcessor_Implementation(TArray<FRule>& NewRules)
{
	UE_LOG(LogTemp, Warning, TEXT("Default AddRuleProcessor_Implementation of MyFunction"));
}


