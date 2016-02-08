// Fill out your copyright notice in the Description page of Project Settings.

#include "TtlExtensionEditorModulePrivatePCH.h"
#include "TEXP_K2Node_ArrayFirstOf.h"
#include "KismetCompiler.h"
#include "CompilerResultsLog.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "TEXP_Array_Library.h"


#define LOCTEXT_NAMESPACE "K2Node_GetDataTableRow"

namespace
{
	struct UTEXP_K2Node_ArrayFirstOfHelper
	{
		static FString ArrayPinName;
		static FString NotFoundPinName;
	};

	FString UTEXP_K2Node_ArrayFirstOfHelper::ArrayPinName(LOCTEXT("DataTablePinName", "DataTable").ToString());
	FString UTEXP_K2Node_ArrayFirstOfHelper::NotFoundPinName(LOCTEXT("RowNotFoundPinName", "RowNotFound").ToString());

}


UTEXP_K2Node_ArrayFirstOf::UTEXP_K2Node_ArrayFirstOf(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Attempts to retrieve a TableRow from a DataTable via it's RowName");
}

void UTEXP_K2Node_ArrayFirstOf::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Thenピンを作成
	CreatePin(EGPD_Input, K2Schema->PC_Exec, TEXT(""), nullptr, false, false, K2Schema->PN_Execute);
	UEdGraphPin* RowFoundPin = CreatePin(EGPD_Output, K2Schema->PC_Exec, TEXT(""), nullptr, false, false, K2Schema->PN_Then);
	RowFoundPin->PinFriendlyName = LOCTEXT("GetDataTableRow Row Found Exec pin", "Row Found");
	CreatePin(EGPD_Output, K2Schema->PC_Exec, TEXT(""), nullptr, false, false, UTEXP_K2Node_ArrayFirstOfHelper::NotFoundPinName);

	// Arrayピンを作成 PC_Wildcard
	UEdGraphPin* DataTablePin = CreatePin(EGPD_Input, K2Schema->PC_Wildcard, TEXT(""), nullptr, false, false, UTEXP_K2Node_ArrayFirstOfHelper::ArrayPinName);
	SetPinToolTip(*DataTablePin, LOCTEXT("DataTablePinDescription", "The DataTable you want to retreive a row from"));

	// Result pin
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, K2Schema->PC_Wildcard, TEXT(""), nullptr, false, false, K2Schema->PN_ReturnValue);
	ResultPin->PinFriendlyName = LOCTEXT("GetDataTableRow Output Row", "Out Row");
	SetPinToolTip(*ResultPin, LOCTEXT("ResultPinDescription", "The returned TableRow, if found"));

	Super::AllocateDefaultPins();
}

void UTEXP_K2Node_ArrayFirstOf::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
{
	MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

	UEdGraphSchema_K2 const* const K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema());
	if (K2Schema != nullptr)
	{
		MutatablePin.PinToolTip += TEXT(" ");
		MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
	}

	MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

void UTEXP_K2Node_ArrayFirstOf::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);
}

void UTEXP_K2Node_ArrayFirstOf::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UTEXP_K2Node_ArrayFirstOf::GetMenuCategory() const
{
	return LOCTEXT("TTLExtension_Array_Category", "TTLExtension | Array");
}

bool UTEXP_K2Node_ArrayFirstOf::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	return false;
	// 接続時のメッセージ
	//OutReason = LOCTEXT("OnlyLiteralDTSupported", "Only literal data table is supported").ToString();
}

void UTEXP_K2Node_ArrayFirstOf::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	return Super::PinDefaultValueChanged(ChangedPin);
}

FText UTEXP_K2Node_ArrayFirstOf::GetTooltipText() const
{
	return NodeTooltip;
}

UEdGraphPin* UTEXP_K2Node_ArrayFirstOf::GetThenPin()const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(K2Schema->PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UTEXP_K2Node_ArrayFirstOf::GetArrayPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UTEXP_K2Node_ArrayFirstOfHelper::ArrayPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UTEXP_K2Node_ArrayFirstOf::GetNotFoundPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UTEXP_K2Node_ArrayFirstOfHelper::NotFoundPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UTEXP_K2Node_ArrayFirstOf::GetResultPin() const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(K2Schema->PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}


FText UTEXP_K2Node_ArrayFirstOf::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("FirstOf", "First Of");
}

void UTEXP_K2Node_ArrayFirstOf::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UTEXP_Array_Library, Array_FirstOf);
	UK2Node_CallFunction* Array_FirstOf_Function = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	Array_FirstOf_Function->FunctionReference.SetExternalMember(FunctionName, UTEXP_Array_Library::StaticClass());
	Array_FirstOf_Function->AllocateDefaultPins();
	CompilerContext.MovePinLinksToIntermediate(*this->GetExecPin(), *(Array_FirstOf_Function->GetExecPin()));

	
	UEdGraphPin* ArrayInPin = Array_FirstOf_Function->FindPinChecked(TEXT("TargetArray"));
	CompilerContext.MovePinLinksToIntermediate(*this->GetArrayPin(), *ArrayInPin);

	// Get some pins to work with
	UEdGraphPin* OriginalOutRowPin = FindPinChecked(Schema->PN_ReturnValue);
	UEdGraphPin* FunctionOutRowPin = Array_FirstOf_Function->FindPinChecked(TEXT("Item"));
	UEdGraphPin* FunctionReturnPin = Array_FirstOf_Function->FindPinChecked(Schema->PN_ReturnValue);
	UEdGraphPin* FunctionThenPin = Array_FirstOf_Function->GetThenPin();

	// Set the type of the OutRow pin on this expanded mode to match original
	FunctionOutRowPin->PinType = OriginalOutRowPin->PinType;
	FunctionOutRowPin->PinType.PinSubCategoryObject = OriginalOutRowPin->PinType.PinSubCategoryObject;


	//BRANCH NODE
	UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	BranchNode->AllocateDefaultPins();
	// Hook up inputs to branch
	FunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
	FunctionReturnPin->MakeLinkTo(BranchNode->GetConditionPin());

	// Hook up outputs
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
	CompilerContext.MovePinLinksToIntermediate(*this->GetNotFoundPin(), *(BranchNode->GetElsePin()));
	CompilerContext.MovePinLinksToIntermediate(*OriginalOutRowPin, *FunctionOutRowPin);

	BreakAllNodeLinks();
}

void UTEXP_K2Node_ArrayFirstOf::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
	return Super::EarlyValidation(MessageLog);
}

#undef LOCTEXT_NAMESPACE