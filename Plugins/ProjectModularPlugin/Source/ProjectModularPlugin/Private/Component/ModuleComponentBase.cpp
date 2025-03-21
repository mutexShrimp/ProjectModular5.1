// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ModuleComponentBase.h"

#include "Class/ModuleClassInterface.h"

// Sets default values for this component's properties
UModuleComponentBase::UModuleComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UModuleComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UModuleComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UModuleComponentBase::BindInvokeEvent_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass)
{
	UE_LOG(LogTemp, Warning, TEXT("Bind %s For Invoke Event"), *ModuleClass.GetObject()->GetName());
	InvokeEventBinders.AddUnique(ModuleClass);
}

TArray<TScriptInterface<IModuleClassInterface>> UModuleComponentBase::GetInvokeEventBinders()
{
	return InvokeEventBinders;
}
