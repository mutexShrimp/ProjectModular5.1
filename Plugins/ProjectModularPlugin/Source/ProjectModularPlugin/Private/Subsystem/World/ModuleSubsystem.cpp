// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/World/ModuleSubsystem.h"

#include "Class/RuleProcessingClassBase.h"
#include "Component/ModuleComponent.h"
#include "Kismet/BlueprintMapLibrary.h"


UModuleSubsystem::UModuleSubsystem()
{
	
}

void UModuleSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (UWorld* World = GetWorld())
	{
		TArray<UObject*> FoundObjects;
		GetObjectsOfClass(ARuleProcessingClassBase::StaticClass(), FoundObjects, true);
		TMap<UClass*, UObject*> Classes;
		for (UObject* FoundObject : FoundObjects)
		{
			Classes.FindOrAdd(FoundObject->GetClass(), FoundObject);
		}
		
		if (Classes.IsEmpty())
		{
			ARuleProcessingClassBase* NewActor = World->SpawnActor<ARuleProcessingClassBase>(ARuleProcessingClassBase::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			Classes.Add(NewActor->GetClass(), NewActor);
		}
		TArray<UClass*> ClassArr;
		Classes.GetKeys(ClassArr);
		UE_LOG(LogTemp, Warning, TEXT("RuleProcessingClass Using %s"), *ClassArr[0]->GetName());
		UObject** Object = Classes.Find(ClassArr[0]);
		RuleProcessingClass = Cast<ARuleProcessingClassBase>(*Object);

		//test
		TArray<UClass*> aaa;
		Classes.GetKeys(aaa);
		for (UObject* FoundObject1 : FoundObjects) 
		{
			RuleProcessingClassArr.Add(Cast<ARuleProcessingClassBase>(FoundObject1));
		}
		
		
		
		if (Classes.Num() > 1)
		{
			UE_LOG(LogTemp, Error, TEXT("Too many RuleProcessingClasses (%d Classes) causing confusion"), Classes.Num());
		}
	}
}

void UModuleSubsystem::BindModule_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass)
{
	UE_LOG(LogTemp, Warning, TEXT("BindModule Function is Invoked by %s"), *ModuleClass.GetObject()->GetName());

	FString ModuleName = ModuleClass->Execute_GetModuleName(ModuleClass.GetObject());
	Modules.Add(ModuleName, ModuleClass);

	UE_LOG(LogTemp, Warning, TEXT("Added %s Module"), *ModuleName);
}

void UModuleSubsystem::InvokeModuleByName_Implementation(const FString& ModuleName, bool& isEmpty)
{
	InvokeModule(*Modules.Find(ModuleName), isEmpty);
}

void UModuleSubsystem::InvokeModuleByClass_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass,
	bool& isEmpty)
{
	InvokeModule(ModuleClass, isEmpty);
}

void UModuleSubsystem::InvokeModule(const TScriptInterface<IModuleClassInterface>& ModuleClass, bool& isEmpty)
{
	UE_LOG(LogTemp, Warning, TEXT("InvokeModule Function is Invoked by %s"), *ModuleClass.GetObject()->GetName());

	FString ModuleName = ModuleClass->Execute_GetModuleName(ModuleClass.GetObject());
	TScriptInterface<IModuleClassInterface>* ModuleClass1 = Modules.Find(ModuleName);
	isEmpty = !IsValid(ModuleClass1->GetObject());
	if (!isEmpty)
	{
		TArray<FString> KeysArray;
		int32 KeysNum = Modules.GetKeys(KeysArray);
		for (TArray<FString>::TIterator It(KeysArray); It; ++It)
		{
			FString& Key = *It;
			TScriptInterface<IModuleClassInterface>* ModuleClass2 = Modules.Find(Key);
			AActor* Actor = Cast<AActor>(ModuleClass2->GetObject());
			UModuleComponent* Component = Actor->FindComponentByClass<UModuleComponent>();
			if (Component && RuleProcessingClass)
			{
				RuleProcessingClass->HandleInvocation(Component);

				for (ARuleProcessingClassBase* ProcessingClassArr : RuleProcessingClassArr)
				{
					ProcessingClassArr->HandleInvocation(Component);
				}
			} 
		}
	}
}




