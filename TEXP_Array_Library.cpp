// Fill out your copyright notice in the Description page of Project Settings.

#include "TtlExtensionModulePrivatePCH.h"
#include "TEXP_Array_Library.h"
#include "Kismet/KismetArrayLibrary.h"


bool UTEXP_Array_Library::Array_FirstOf(const TArray<int32>& TargetArray, int32& Item)
{
	check(0);
	return false;
}


bool UTEXP_Array_Library::GenericArray_FirstOf(void* TargetArray, const UArrayProperty* ArrayProp, void* Item)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		UProperty* InnerProp = ArrayProp->Inner;
		if (ArrayHelper.IsValidIndex(0))
		{
			InnerProp->CopyCompleteValueFromScriptVM(Item, ArrayHelper.GetRawPtr(0));
			return true;
		}
		else
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("Attempted to get an item from array %s out of bounds [%d/%d]!"), *ArrayProp->GetName(), 0, UKismetArrayLibrary::GetLastIndex(ArrayHelper)), ELogVerbosity::Warning);
			InnerProp->InitializeValue(Item);
		}
	}
	return false;
}