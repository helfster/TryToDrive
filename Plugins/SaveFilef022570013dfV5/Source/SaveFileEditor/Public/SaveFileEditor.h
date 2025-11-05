// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class SDockTab;
class SSaveFileEditorTab;

class FSaveFileEditorModule : public IModuleInterface {
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static FSaveFileEditorModule& Get() {
        return FModuleManager::GetModuleChecked<FSaveFileEditorModule>("SaveFileEditor");
    }

    TWeakPtr<SSaveFileEditorTab> CurrentSaveFileEditorTab;

    /** This function will be bound to Command (by default it will bring up plugin window) */
    void PluginButtonClicked();

    // Deafult save directory
    FORCEINLINE static const FString GetDefaultSaveDirectory() { return FPaths::ProjectSavedDir() / TEXT("SaveGames/"); }

    // Found all *.sav file inside a directory
    static bool GetSaveGameNames(TSet<FString>& FoundSaves, FString SaveDirectory = TEXT(""));

    TSharedPtr<FTabManager> GetSaveEditorTabManager() { return SaveEditorTabManager; }

    bool IsGameStarted() { return bGameStarted; }

    static bool IsTabVisibleGlobal(FName InTabName) {
        return FGlobalTabmanager::Get()->FindExistingLiveTab(InTabName).IsValid();
    };

    static bool IsTabVisible(TWeakPtr<FTabManager> InDebuggingToolsManagerWeak, FName InTabName) {
        TSharedPtr<FTabManager> InDebuggingToolsManager = InDebuggingToolsManagerWeak.Pin();
        if (InDebuggingToolsManager.IsValid()) {
            return InDebuggingToolsManager->FindExistingLiveTab(InTabName).IsValid();
        }
        return false;
    };

    static void ToggleTabVisibility(TWeakPtr<FTabManager> InDebuggingToolsManagerWeak, FName InTabName) {
        TSharedPtr<FTabManager> InDebuggingToolsManager = InDebuggingToolsManagerWeak.Pin();
        if (InDebuggingToolsManager.IsValid()) {
            TSharedPtr<SDockTab> ExistingTab = InDebuggingToolsManager->FindExistingLiveTab(InTabName);
            if (ExistingTab.IsValid()) {
                ExistingTab->RequestCloseTab();
            } else {
                InDebuggingToolsManager->TryInvokeTab(InTabName);
            }
        }
    };

private:
    void RegisterMenus();

    TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

    // virtual void MakeMenuEntry(FMenuBuilder& menuBuilder);

    void OnPieWorldStarted(bool bIsSimulated);
    void OnPieWorldStopped(bool bIsSimulated);

private:
    bool bGameStarted = false;

    FDelegateHandle PostPIEStartedHandle;
    FDelegateHandle EndPIEHandle;
    FDelegateHandle PausePIEHandle;
    FDelegateHandle ResumePIEHandle;

    TSharedPtr<class FUICommandList> PluginCommands;
    TWeakPtr<SDockTab> SaveEditorManagerTab;

    // Manage all tab controls
    TSharedPtr<FTabManager> SaveEditorTabManager;

    // All tab level management
    TSharedPtr<FTabManager::FLayout> SaveEditorTabLayout;

    TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
    TSharedPtr<FExtender> MenuExtender;
};
