// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/ModuleEventCallableInterface.h"

// Add default functionality here for any IModuleClassInterface functions that are not pure virtual.

FString IModuleEventCallableInterface::GetModuleName_Implementation()
{
	return FString();
}

void IModuleEventCallableInterface::ModuleEvent_Implementation(const FString& ModuleName, const FString& PrevModuleName, const FString& EventName)
{
}

