// Copyright Hubber Studios 2023. All Rights Reserved.

#include "SaveFileEditor.h"
#include "HAL/FileManagerGeneric.h"
#include "LevelEditor.h"
#include "Misc/FileHelper.h"
#include "SaveFileEditorCommands.h"
#include "SaveFileEditorStyle.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"

#include "SSaveFileEditorTab.h"
#include "SSaveFileManagerTab.h"

static const FName SaveFileEditorTabName("SaveFileWindow");

#define LOCTEXT_NAMESPACE "FSaveFileEditorModule"

void FSaveFileEditorModule::StartupModule() {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    FSaveFileEditorStyle::Initialize();
    FSaveFileEditorStyle::ReloadTextures();

    FSaveFileEditorCommands::Register();

    // Create plugin commands
    PluginCommands = MakeShareable(new FUICommandList);

    // Action to open plugins tabs
    PluginCommands->MapAction(
        FSaveFileEditorCommands::Get().OpenPluginWindow,
        FExecuteAction::CreateRaw(this, &FSaveFileEditorModule::PluginButtonClicked),
        FCanExecuteAction(),
        FIsActionChecked::CreateStatic(
            &FSaveFileEditorModule::IsTabVisibleGlobal,
            SaveFileEditorTabName));

    // Register hubber utils menu
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSaveFileEditorModule::RegisterMenus));

    // Register plugin tab
    FGlobalTabmanager::Get()
        ->RegisterNomadTabSpawner(SaveFileEditorTabName, FOnSpawnTab::CreateRaw(this, &FSaveFileEditorModule::OnSpawnPluginTab))
        .SetDisplayName(LOCTEXT("FSaveFileEditorTabTitle", "Save Files"))
        .SetTooltipText(LOCTEXT("FSaveFileEditorTabToolTip", "All tools for the save file editor."))
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Save"))
        .SetAutoGenerateMenuEntry(false);

    PostPIEStartedHandle = FEditorDelegates::PostPIEStarted.AddRaw(this, &FSaveFileEditorModule::OnPieWorldStarted);
    EndPIEHandle = FEditorDelegates::EndPIE.AddRaw(this, &FSaveFileEditorModule::OnPieWorldStopped);

    PausePIEHandle = FEditorDelegates::PausePIE.AddRaw(this, &FSaveFileEditorModule::OnPieWorldStopped);
    ResumePIEHandle = FEditorDelegates::ResumePIE.AddRaw(this, &FSaveFileEditorModule::OnPieWorldStarted);
}

void FSaveFileEditorModule::ShutdownModule() {
    UToolMenus::UnRegisterStartupCallback(this);

    UToolMenus::UnregisterOwner(this);

    FSaveFileEditorStyle::Shutdown();

    FSaveFileEditorCommands::Unregister();

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SaveFileEditorTabName);

    if (SaveEditorTabManager.IsValid()) {
        FGlobalTabmanager::Get()->UnregisterTabSpawner(SaveFileEditorTabName);
        SaveEditorTabLayout = TSharedPtr<FTabManager::FLayout>();
        SaveEditorTabManager = TSharedPtr<FTabManager>();
    }

    FSaveFileEditorCommands::Unregister();

    FEditorDelegates::PostPIEStarted.Remove(PostPIEStartedHandle);
    FEditorDelegates::PostPIEStarted.Remove(EndPIEHandle);
    FEditorDelegates::PostPIEStarted.Remove(PausePIEHandle);
    FEditorDelegates::PostPIEStarted.Remove(ResumePIEHandle);
}

TSharedRef<SDockTab> FSaveFileEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs) {
    // Create SDockTab
    const TSharedRef<SDockTab> NomadTab = SAssignNew(SaveEditorManagerTab, SDockTab)
                                              .TabRole(ETabRole::NomadTab);

    if (!SaveEditorTabManager.IsValid()) {
        SaveEditorTabManager = FGlobalTabmanager::Get()->NewTabManager(NomadTab);
        SaveEditorTabManager->SetOnPersistLayout(
            FTabManager::FOnPersistLayout::CreateStatic(
                [](const TSharedRef<FTabManager::FLayout>& InLayout) {
                    if (InLayout->GetPrimaryArea().Pin().IsValid()) {
                        FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, InLayout);
                    }
                }));
    } else {
        ensure(SaveEditorTabLayout.IsValid());
    }

    const FName SaveEditorManagerName = SSaveFileManagerTab::GetTabName();
    const FName SaveEditorEditName = SSaveFileEditorTab::GetTabName();

    // Save tab layout after close
    TWeakPtr<FTabManager> SaveEditorTabManagerWeak = SaveEditorTabManager;
    NomadTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateStatic(
        [](TSharedRef<SDockTab> Self, TWeakPtr<FTabManager> TabManager) {
            TSharedPtr<FTabManager> OwningTabManager = TabManager.Pin();
            if (OwningTabManager.IsValid()) {
                FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, OwningTabManager->PersistLayout());
                OwningTabManager->CloseAllAreas();
            }
        },
        SaveEditorTabManagerWeak));

    if (!SaveEditorTabLayout.IsValid()) {

        SaveEditorTabManager->RegisterTabSpawner(SaveEditorManagerName, FOnSpawnTab::CreateStatic(SSaveFileManagerTab::OnSpawnContentTab))
            .SetDisplayName(LOCTEXT("TabTitle", "Save Files Manager"))
            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Outliner"));

        SaveEditorTabManager->RegisterTabSpawner(SaveEditorEditName, FOnSpawnTab::CreateStatic(SSaveFileEditorTab::OnSpawnContentTab))
            .SetDisplayName(LOCTEXT("TabTitle", "Save File Editor"))
            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Save"));

        SaveEditorTabLayout = FTabManager::NewLayout("Standalone_SaveEditor_Layout_v1")
                                  ->AddArea(
                                      FTabManager::NewPrimaryArea()
                                          ->SetOrientation(Orient_Horizontal)
                                          ->Split(
                                              FTabManager::NewStack()
                                                  ->SetSizeCoefficient(.4f)
                                                  ->SetHideTabWell(true)
                                                  ->AddTab(SaveEditorManagerName, ETabState::OpenedTab)
                                                  ->AddTab(SaveEditorEditName, ETabState::OpenedTab)
                                                  ->SetForegroundTab(SaveEditorManagerName)));
    }

    SaveEditorTabLayout = FLayoutSaveRestore::LoadFromConfig(GEditorLayoutIni, SaveEditorTabLayout.ToSharedRef());

    TSharedRef<SWidget> TabContents = SaveEditorTabManager->RestoreFrom(SaveEditorTabLayout.ToSharedRef(), TSharedPtr<SWindow>()).ToSharedRef();

    PluginCommands->MapAction(
        FSaveFileEditorCommands::Get().OpenSaveManagerWindow,
        FExecuteAction::CreateStatic(
            &FSaveFileEditorModule::ToggleTabVisibility,
            SaveEditorTabManagerWeak,
            SaveEditorManagerName),
        FCanExecuteAction::CreateStatic(
            []() { return true; }),
        FIsActionChecked::CreateStatic(
            &FSaveFileEditorModule::IsTabVisible,
            SaveEditorTabManagerWeak,
            SaveEditorManagerName));

    PluginCommands->MapAction(
        FSaveFileEditorCommands::Get().OpenSaveEditorWindow,
        FExecuteAction::CreateStatic(
            &FSaveFileEditorModule::ToggleTabVisibility,
            SaveEditorTabManagerWeak,
            SaveEditorEditName),
        FCanExecuteAction::CreateStatic(
            []() { return true; }),
        FIsActionChecked::CreateStatic(
            &FSaveFileEditorModule::IsTabVisible,
            SaveEditorTabManagerWeak,
            SaveEditorEditName));

    TWeakPtr<SWidget> OwningWidgetWeak = NomadTab;
    TabContents->SetOnMouseButtonUp(
        FPointerEventHandler::CreateStatic(
            [](/** The geometry of the widget*/
                const FGeometry&,
                /** The Mouse Event that we are processing */
                const FPointerEvent& PointerEvent,
                TWeakPtr<SWidget> InOwnerWeak,
                TSharedPtr<FUICommandList> InCommandList) -> FReply {
                if (PointerEvent.GetEffectingButton() == EKeys::RightMouseButton) {
                    // if the tab manager is still available then make a context window that allows users to
                    // show and hide tabs:
                    TSharedPtr<SWidget> InOwner = InOwnerWeak.Pin();
                    if (InOwner.IsValid()) {
                        FMenuBuilder MenuBuilder(true, InCommandList);

                        MenuBuilder.PushCommandList(InCommandList.ToSharedRef());
                        {
                            MenuBuilder.AddMenuEntry(FSaveFileEditorCommands::Get().OpenSaveManagerWindow);
                            MenuBuilder.AddMenuEntry(FSaveFileEditorCommands::Get().OpenSaveEditorWindow);
                        }
                        MenuBuilder.PopCommandList();

                        FWidgetPath WidgetPath = PointerEvent.GetEventPath() != nullptr ? *PointerEvent.GetEventPath() : FWidgetPath();
                        FSlateApplication::Get().PushMenu(InOwner.ToSharedRef(), WidgetPath, MenuBuilder.MakeWidget(), PointerEvent.GetScreenSpacePosition(), FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));

                        return FReply::Handled();
                    }
                }

                return FReply::Unhandled();
            },
            OwningWidgetWeak, PluginCommands));

    NomadTab->SetContent(
        SNew(SBorder)
            .BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
            .Padding(FMargin(0.f, 2.f))
                [TabContents]);

    TSharedPtr<SDockTab> SaveEditorTab = SaveEditorTabManager->FindExistingLiveTab(SSaveFileEditorTab::GetTabName());
    if (!SaveEditorTab.IsValid()) {
        SaveEditorTab = SaveEditorTabManager->TryInvokeTab(SSaveFileEditorTab::GetTabName());
        if (SaveEditorTab) {
            SaveEditorTabManager->DrawAttention(SaveEditorTab.ToSharedRef());
            SaveEditorTab->ActivateInParent(ETabActivationCause::SetDirectly);
        }
    }

    return NomadTab;
}

void FSaveFileEditorModule::OnPieWorldStarted(bool bIsSimulating) {
    bGameStarted = true;
}

void FSaveFileEditorModule::OnPieWorldStopped(bool bIsSimulating) {
    bGameStarted = false;
}

void FSaveFileEditorModule::PluginButtonClicked() {
    FGlobalTabmanager::Get()->TryInvokeTab(SaveFileEditorTabName);
}

bool FSaveFileEditorModule::GetSaveGameNames(TSet<FString>& FoundSaves, FString SaveDirectory) {
    TArray<FString> FoundFiles;
    const FString SaveGameDirectory = SaveDirectory.IsEmpty() ? GetDefaultSaveDirectory() : SaveDirectory;
    IFileManager::Get().FindFilesRecursive(FoundFiles, *SaveGameDirectory, TEXT("*.sav"), true, false);

    FoundSaves.Empty(FoundFiles.Num());

    for (FString& File : FoundFiles) {
        FPaths::RemoveDuplicateSlashes(File);
        File.RemoveFromStart(SaveGameDirectory);
        FoundSaves.Add(File);
    }

    return true;
}

void FSaveFileEditorModule::RegisterMenus() {
    FToolMenuOwnerScoped OwnerScoped(this);
    {
        // Get hubber tools menu or create if not exists
        UToolMenu* HubberToolsMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu.HubberTools");
        if (!HubberToolsMenu) {
            UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
            HubberToolsMenu = MainMenu->AddSubMenuScript(FName("HubberTools"), FName("Tools"), FName("HubberTools"), FText::FromString(TEXT("Hubber Tools")));
        }

        // Bind hubber tools menu to tab save file category
        HubberToolsMenu->AddDynamicSection("SaveFileSection", FNewToolMenuDelegateLegacy::CreateLambda([](FMenuBuilder& InBuilder, UToolMenu* InData) {
            if (USlateTabManagerContext* TabManagerContext = InData->FindContext<USlateTabManagerContext>()) {
                TSharedPtr<FTabManager> TabManager = TabManagerContext->TabManager.Pin();
                if (TabManager.IsValid()) {
                    TabManager->PopulateTabSpawnerMenu(InBuilder, SaveFileEditorTabName);
                }
            }
        }));
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSaveFileEditorModule, SaveFileEditor)