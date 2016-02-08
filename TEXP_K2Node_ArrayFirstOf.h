// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "K2Node.h"
#include "EdGraph/EdGraphNodeUtils.h" // for FNodeTextCache
#include "TEXP_K2Node_ArrayFirstOf.generated.h"

/**
 * 
 */
UCLASS()
class TTLEXTENSIONEDITORMODULE_API UTEXP_K2Node_ArrayFirstOf : public UK2Node
{
	GENERATED_UCLASS_BODY()
	
	
public:
	
	// Begin UEdGraphNode interface.

	/// <summary>
	/// 入力/出力ピン情報を作成します。
	/// <para>SpawnActor では In/Out 実行ピンの他、以下のピンが入力ピンとして定義されています。</para>
	/// </summary>
	virtual void AllocateDefaultPins() override;
	/// <summary>
	/// ノードのタイトルを指定します。タイトルを入力クラスに合わせて動的に変更させることも可能です。
	/// </summary>
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	/// <summary>
	/// ピンデフォルト値を変更された際のコールバック
	/// </summary>
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FText GetTooltipText() const override;
	/// <summary>
	/// ノード動作を確定させます
	/// </summary>
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FName GetPaletteIcon(FLinearColor& OutColor) const override
	{
		OutColor = GetNodeTitleColor();
		return TEXT("Kismet.AllClasses.FunctionIcon");
	}
	// End UEdGraphNode interface.

	// Begin UK2Node interface
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	/// <summary>
	/// 何かしらのタイミングで入力/出力ピン情報に変更が出た場合の処理です。
	/// </summary>
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	/// <summary>
	/// メニューカテゴリー
	/// </summary>
	virtual FText GetMenuCategory() const override;
	/// <summary>
	/// 接続が許可されていないか判定する
	/// </summary>
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	/// <summary>
	/// 早期検証
	/// </summary>
	virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
	// End UK2Node interface


	/** Get the then output pin */
	UEdGraphPin* GetThenPin() const;
	/** Get the Data Table input pin */
	UEdGraphPin* GetArrayPin() const;
	/** Get the exec output pin for when the row was not found */
	UEdGraphPin* GetNotFoundPin() const;
	/** Get the result output pin */
	UEdGraphPin* GetResultPin() const;


private:
	/**
	* Takes the specified "MutatablePin" and sets its 'PinToolTip' field (according
	* to the specified description)
	*
	* @param   MutatablePin	The pin you want to set tool-tip text on
	* @param   PinDescription	A string describing the pin's purpose
	*/
	void SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const;

	/** Tooltip text for this node. */
	FText NodeTooltip;

	
};
