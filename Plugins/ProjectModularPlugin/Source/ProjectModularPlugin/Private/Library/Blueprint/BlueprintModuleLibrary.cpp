// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/Blueprint/BlueprintModuleLibrary.h"
#include "ProjectModularPlugin.h"

UBlueprintModuleLibrary::UBlueprintModuleLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UBlueprintModuleLibrary::ProjectModularPluginSampleFunction(float Param)
{
	return -1;
}