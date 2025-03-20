// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/ModuleClassInterface.h"

// Add default functionality here for any IModuleClassInterface functions that are not pure virtual.

FString IModuleClassInterface::GetModuleName_Implementation()
{
	return FString();
}

void IModuleClassInterface::ModuleEvent_Implementation(const FString& EventName)
{
}

