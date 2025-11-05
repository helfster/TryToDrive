// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/SCompoundWidget.h"

class USaveGame;
class SWidgetSwitcher;
class IDetailsView;
class STextBlock;

class SSaveFileEditorTab : public SCompoundWidget {

    SLATE_BEGIN_ARGS(SSaveFileEditorTab) { }
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    ~SSaveFileEditorTab();

    static TSharedPtr<SOverlay> TestSaveEditor;

    static FName GetTabName() { return FName(TEXT("SaveFileEditorEdit")); }
    static TSharedRef<class SDockTab> OnSpawnContentTab(const class FSpawnTabArgs& SpawnTabArgs);

    void LoadObjectFromDisk(FString File);
    void ClearEditedFile();

    void OnPieWorldStarted(bool bIsSimulated);
    void OnPieWorldStopped(bool bIsSimulated);

    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

    FString GetCurrentFile() { return File; }

private:
    TSharedPtr<STextBlock> FileNameText;
    TSharedPtr<IDetailsView> PropertyWidget;
    FString File;

    FDateTime FileTimeStamp;

    FDelegateHandle ObjectEditHandle;

    FDelegateHandle PostPIEStartedHandle;
    FDelegateHandle EndPIEHandle;
    FDelegateHandle PausePIEHandle;
    FDelegateHandle ResumePIEHandle;

    USaveGame* SaveGameObject;

    bool bIsEditable = true;

    void OnObjectEdit(UObject* Obj, FPropertyChangedEvent& Property);

    bool IsObjectEditable() { return bIsEditable; }

    void ClearEditedObject();
};
