// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Core.h"
#include "UObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TEXP_Array_Library.generated.h"

/**
 * 
 */
UCLASS()
class TTLEXTENSIONMODULE_API UTEXP_Array_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:


	/**
	*Add item to array
	*
	*@param	TargetArray		The array to add item to
	*@param	NewItem			The item to add to the array
	*@return	The index of the newly added item
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "FindFirstOf", ArrayParm = "TargetArray", ArrayTypeDependentParams = "Item"), Category = "Utilities|Array")
	static bool Array_FirstOf(const TArray<int32>& TargetArray, int32& Item);
	
	
	static bool GenericArray_FirstOf(void* TargetArray, const UArrayProperty* ArrayProp, void* Item);


	DECLARE_FUNCTION(execArray_FirstOf)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(nullptr);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		// Since Item isn't really an int, step the stack manually
		const UProperty* InnerProp = ArrayProperty->Inner;
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
		void* StorageSpace = FMemory_Alloca(PropertySize);
		InnerProp->InitializeValue(StorageSpace);

		Stack.MostRecentPropertyAddress = NULL;
		Stack.StepCompiledIn<UProperty>(StorageSpace);
		void* ItemPtr = (Stack.MostRecentPropertyAddress != nullptr) ? Stack.MostRecentPropertyAddress : StorageSpace;

		P_FINISH;

		*(bool*)RESULT_PARAM = GenericArray_FirstOf(ArrayAddr, ArrayProperty, ItemPtr);

		InnerProp->DestroyValue(StorageSpace);
	}
	
};
