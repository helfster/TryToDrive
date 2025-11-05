// Copyright Hubber Studios 2023. All Rights Reserved.

#include "HubberUtils/Tab/TabToolBase.h"

#include "Framework/Docking/TabManager.h"
#include "ToolMenuContext.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/SCompoundWidget.h"

void FTabToolBase::OnStartupModule() {
    Initialize();
    FTabSpawnerEntry& TabEntry
        = FGlobalTabmanager::Get()
              ->RegisterNomadTabSpawner(TabMenuName,
                  FOnSpawnTab::CreateRaw(this, &FTabToolBase::SpawnTab))
              .SetDisplayName(TabDisplayName)
              .SetTooltipText(ToolTipText)
              .SetAutoGenerateMenuEntry(false);

    if (Icon) {
        TabEntry.SetIcon(*Icon);
    }

    if (TabMenuName.IsValid() && OpenTabCommand.IsValid()) {
        ModuleOwner->GetPluginCommands()->MapAction(
            OpenTabCommand,
            FExecuteAction::CreateRaw(this, &FTabToolBase::OpenTabCommandClicked),
            FCanExecuteAction(),
            FIsActionChecked::CreateStatic(&FTabToolBase::IsTabVisibleGlobal, TabMenuName));

        UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTabToolBase::RegisterMenus));
    }
}

void FTabToolBase::OnShutdownModule() {
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}

TSharedRef<SDockTab> FTabToolBase::SpawnTab(const FSpawnTabArgs& TabSpawnArgs) {
    TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).TabRole(ETabRole::NomadTab);
    RootWidget = GetRootWidget();

    if (RootWidget.IsValid()) {
        SpawnedTab->SetContent(RootWidget.ToSharedRef());
    }

    return SpawnedTab;
}

void FTabToolBase::MakeMenuEntry(FMenuBuilder& menuBuilder) {
    FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(menuBuilder, TabName);
}

void FTabToolBase::OpenTabCommandClicked() {
    FGlobalTabmanager::Get()->TryInvokeTab(TabMenuName);
}

bool FTabToolBase::IsTabVisibleGlobal(FName InTabName) {
    return FGlobalTabmanager::Get()->FindExistingLiveTab(InTabName).IsValid();
}

void FTabToolBase::RegisterMenus() {
    FToolMenuOwnerScoped OwnerScoped(this);
    {
        // Get hubber tools menu or create if not exists
        UToolMenu* HubberToolsMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu.HubberTools");
        if (!HubberToolsMenu) {
            UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
            HubberToolsMenu = MainMenu->AddSubMenuScript(FName("HubberTools"), FName("Tools"), FName("HubberTools"), FText::FromString(TEXT("Hubber Tools")));
        }

        // Bind hubber tools menu to tab save file category
        FString MenuTabSection = TabMenuName.ToString() + TEXT("Section");
        HubberToolsMenu->AddDynamicSection(FName(*MenuTabSection),
            FNewToolMenuDelegateLegacy::CreateStatic(&FTabToolBase::PopulateHubberMenu, TabMenuName));
    }
}

void FTabToolBase::PopulateHubberMenu(FMenuBuilder& InBuilder, UToolMenu* InData, FName TabName) {
    if (USlateTabManagerContext* TabManagerContext = InData->FindContext<USlateTabManagerContext>()) {
        TSharedPtr<FTabManager> TabManager = TabManagerContext->TabManager.Pin();
        if (TabManager.IsValid()) {
            TabManager->PopulateTabSpawnerMenu(InBuilder, TabName);
        }
    }
}