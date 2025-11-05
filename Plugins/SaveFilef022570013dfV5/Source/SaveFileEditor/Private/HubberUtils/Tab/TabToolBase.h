// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "HubberUtils/Module/IModuleExtensionInterface.h"

/**
 * Base tab for editor plugin.
 * Use function Initialize to set TabMenuName/TabName/TabDisplayName/ToolTipText
 * and to create the root wiget for the tab (Assign to RootWidget)
 */

class FTabToolBase : public IModuleListenerInterface, public TSharedFromThis<FTabToolBase> {
public:
    virtual ~FTabToolBase() {};

    // IPixelopusToolBase
    virtual void OnStartupModule() override;

    virtual void OnShutdownModule() override;

    /* In this function set TabMenuName/TabName/TabDisplayName/ToolTipText */
    virtual void Initialize() = 0;
    virtual TSharedRef<SWidget> GetRootWidget() = 0;

    /* Called to spawn the new tab, return a SDockTab */
    virtual TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& TabSpawnArgs);

    virtual void MakeMenuEntry(FMenuBuilder& menuBuilder);

    void OpenTabCommandClicked();

    static bool IsTabVisibleGlobal(FName InTabName);

    void RegisterMenus();

    static void PopulateHubberMenu(FMenuBuilder& InBuilder, UToolMenu* InData, FName TabName);

protected:
    // Fill in Initialize()
    FName TabMenuName = NAME_None;
    FName TabName = TEXT("BaseTab");
    FText TabDisplayName = FText::FromString(TEXT("BaseTabName"));
    FText ToolTipText = FText::FromString(TEXT("Base Tab Tooltip"));
    FSlateIcon* Icon = nullptr;
    TSharedPtr<FUICommandInfo> OpenTabCommand;

private:
    TSharedPtr<SWidget> RootWidget;
};
