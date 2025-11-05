// Copyright Hubber Studios 2023. All Rights Reserved.

#include "SSaveFileManagerTab.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "PlatformFeatures.h"
#include "SaveGameSystem.h"
#include "Styling/ISlateStyle.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"

#include "GameFramework/SaveGame.h"
#include "SaveFileEditorStyle.h"
#include "SaveFileTypes.h"

#include "SSaveFileEditorTab.h"
#include "SaveFileEditor.h"

#define LOCTEXT_NAMESPACE "FSaveFileEditorModule"

void SSaveFileManagerTab::Construct(const FArguments& InArgs) {
    CurrentSaveDirectory = FPaths::ConvertRelativePathToFull(FSaveFileEditorModule::GetDefaultSaveDirectory());

    this->ChildSlot
        [SNew(SOverlay)
            + SOverlay::Slot()
                  .HAlign(EHorizontalAlignment::HAlign_Fill)
                  .VAlign(EVerticalAlignment::VAlign_Fill)
                  .Padding(10.0f)
                      [SNew(SVerticalBox)
                          + SVerticalBox::Slot()
                                .AutoHeight()
                                .Padding(0.0f, 0.0f, 0.0f, 5.0f)
                                .HAlign(EHorizontalAlignment::HAlign_Fill)
                                .VAlign(EVerticalAlignment::VAlign_Top)
                                    [SNew(SOverlay)
                                        + SOverlay::Slot()
                                              .HAlign(EHorizontalAlignment::HAlign_Fill)
                                              .VAlign(EVerticalAlignment::VAlign_Top)
                                              .Padding(0.0f)
                                                  [SNew(SHorizontalBox)
                                                      + SHorizontalBox::Slot()
                                                            .AutoWidth()
                                                            .HAlign(EHorizontalAlignment::HAlign_Fill)
                                                            .VAlign(EVerticalAlignment::VAlign_Center)
                                                            .Padding(0.0f, 0.0f, 5.0f, 0.0f)
                                                                [SNew(STextBlock)
                                                                        .Font(FSaveFileEditorStyle::Get().GetFontStyle("SaveFileEditor.BoldFont"))
                                                                        .Text(LOCTEXT("SaveDirectory", "Save Directory:"))]
                                                      + SHorizontalBox::Slot()
                                                            .FillWidth(1.0f)
                                                            .HAlign(EHorizontalAlignment::HAlign_Left)
                                                            .VAlign(EVerticalAlignment::VAlign_Center)
                                                            .Padding(0.0f)
                                                                [SAssignNew(SaveDirectoryTextBlock, STextBlock)
                                                                        .Font(FSaveFileEditorStyle::Get().GetFontStyle("SaveFileEditor.NormalFont"))
                                                                        .OverflowPolicy(ETextOverflowPolicy::Ellipsis)
                                                                        .ToolTipText(FText::FromString(CurrentSaveDirectory))
                                                                        .Text(FText::FromString(CurrentSaveDirectory))]
                                                      + SHorizontalBox::Slot()
                                                            .AutoWidth()
                                                            .HAlign(EHorizontalAlignment::HAlign_Right)
                                                            .VAlign(EVerticalAlignment::VAlign_Center)
                                                            .Padding(10.0f, 0.0f, 5.0f, 0.0f)
                                                                [SNew(SButton)
                                                                        .OnClicked(this, &SSaveFileManagerTab::OpenSaveDirectoryClick)
                                                                        .Text(LOCTEXT("OpenSaveDirectory", "Open..."))]
                                                      + SHorizontalBox::Slot()
                                                            .AutoWidth()
                                                            .HAlign(EHorizontalAlignment::HAlign_Right)
                                                            .VAlign(EVerticalAlignment::VAlign_Center)
                                                            .Padding(10.0f, 0.0f, 5.0f, 0.0f)
                                                                [SNew(SButton)
                                                                        .OnClicked(this, &SSaveFileManagerTab::ChangeSaveDirectoryClick)
                                                                        .Text(LOCTEXT("ChangeSaveDirectory", "Change..."))]
                                                      + SHorizontalBox::Slot()
                                                            .AutoWidth()
                                                            .HAlign(EHorizontalAlignment::HAlign_Right)
                                                            .VAlign(EVerticalAlignment::VAlign_Center)
                                                            .Padding(10.0f, 0.0f, 5.0f, 0.0f)
                                                                [SNew(SButton)
                                                                        .OnClicked(this, &SSaveFileManagerTab::ResetsSaveDirectoryClick)
                                                                        .Text(LOCTEXT("ResetSaveDirectory", "Reset to default"))]]]
                          + SVerticalBox::Slot()
                                .FillHeight(1.0f)
                                .HAlign(EHorizontalAlignment::HAlign_Fill)
                                .VAlign(EVerticalAlignment::VAlign_Fill)
                                    [SAssignNew(FilesListContainer, SSaveFileTable)]
                          + SVerticalBox::Slot()
                                .AutoHeight()
                                .Padding(0.0f, 0.0f, 0.0f, 5.0f)
                                .HAlign(EHorizontalAlignment::HAlign_Fill)
                                .VAlign(EVerticalAlignment::VAlign_Top)
                                    [SNew(SOverlay)
                                        + SOverlay::Slot()
                                              .HAlign(EHorizontalAlignment::HAlign_Fill)
                                              .VAlign(EVerticalAlignment::VAlign_Top)
                                              .Padding(0.0f)
                                                  [SNew(SHorizontalBox)
                                                      + SHorizontalBox::Slot()
                                                            .AutoWidth()
                                                            .HAlign(EHorizontalAlignment::HAlign_Right)
                                                            .VAlign(EVerticalAlignment::VAlign_Center)
                                                            .Padding(10.0f, 0.0f, 5.0f, 0.0f)
                                                                [SNew(SButton)
                                                                        .OnClicked(this, &SSaveFileManagerTab::DeleteAllSaveFilesClick)
                                                                        .Text(LOCTEXT("DeleteAllSaveFile", "Delete All"))]]]]];

    UpdateSaveFileList();

    RegisterActiveTimer(2.0f, FWidgetActiveTimerDelegate::CreateSP(this, &SSaveFileManagerTab::TimerUpdate));
}

TSharedRef<class SDockTab> SSaveFileManagerTab::OnSpawnContentTab(const FSpawnTabArgs& SpawnTabArgs) {
    TSharedPtr<SSaveFileManagerTab> SaveFileManager;
    const TSharedRef<SDockTab> Tab = SNew(SDockTab)
                                         .TabRole(ETabRole::PanelTab)
                                         .Label(LOCTEXT("SaveFileManagerTabTitle", "Save Files Manager"))
                                             [SNew(SBorder)
                                                     .BorderImage(FAppStyle::GetBrush("Docking.Tab.ContentAreaBrush"))
                                                     .BorderBackgroundColor(FSlateColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.f)))
                                                         [SAssignNew(SaveFileManager, SSaveFileManagerTab)]];

    return Tab;
}

void SSaveFileManagerTab::UpdateSaveFileList() {
    TSet<FString> NewSaveFiles;
    FSaveFileEditorModule::GetSaveGameNames(NewSaveFiles, CurrentSaveDirectory);

    if (NewSaveFiles.Num() == SaveFiles.Num()) {
        for (FString& File : NewSaveFiles) {
            if (!SaveFiles.Contains(File)) {
                break;
            }
        }
        return;
    }

    SaveFiles = NewSaveFiles;
    FilesListContainer->UpdateFilesList(CurrentSaveDirectory, SaveFiles);
}

FReply SSaveFileManagerTab::OpenSaveDirectoryClick() {
    FPlatformProcess::ExploreFolder(*CurrentSaveDirectory);
    return FReply::Handled();
}

FReply SSaveFileManagerTab::ChangeSaveDirectoryClick() {
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    FString NewFolder;
    const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr), TEXT("Choose new save directory"), CurrentSaveDirectory, NewFolder);
    if (bFolderSelected) {
        FPaths::ConvertRelativePathToFull(NewFolder);
        CurrentSaveDirectory = NewFolder;
        SaveDirectoryTextBlock->SetText(FText::FromString(CurrentSaveDirectory));
        UpdateSaveFileList();
    }
    return FReply::Handled();
}

FReply SSaveFileManagerTab::ResetsSaveDirectoryClick() {
    CurrentSaveDirectory = FPaths::ConvertRelativePathToFull(FSaveFileEditorModule::GetDefaultSaveDirectory());
    SaveDirectoryTextBlock->SetText(FText::FromString(CurrentSaveDirectory));
    UpdateSaveFileList();
    return FReply::Handled();
}

FReply SSaveFileManagerTab::DeleteAllSaveFilesClick() {

    EAppReturnType::Type ReturnDialog = FMessageDialog::Open(EAppMsgType::OkCancel, EAppReturnType::Cancel, LOCTEXT("ConfirmDeleteSaveFile", "Are you sure?"));

    if (ReturnDialog != EAppReturnType::Ok) {
        return FReply::Handled();
    }

    IFileManager& FileManager = IFileManager::Get();
    for (FString& File : SaveFiles) {
        FString FullFile = FPaths::Combine(CurrentSaveDirectory, File);
        if (FPaths::ValidatePath(FullFile) && FPaths::FileExists(FullFile)) {
            FileManager.Delete(*FullFile);
        }

        TWeakPtr<SSaveFileEditorTab> CurrentSaveFileEditorTab = FSaveFileEditorModule::Get().CurrentSaveFileEditorTab;
        if (CurrentSaveFileEditorTab.IsValid()) {
            if (CurrentSaveFileEditorTab.Pin()->GetCurrentFile() == FullFile) {
                CurrentSaveFileEditorTab.Pin()->ClearEditedFile();
            }
        }
    }
    UpdateSaveFileList();
    return FReply::Handled();
}

void SSaveFileManagerTab::SaveFileDeleted(FString File) {
    UpdateSaveFileList();
}

EActiveTimerReturnType SSaveFileManagerTab::TimerUpdate(double InCurrentTime, float DeltaTime) {
    UpdateSaveFileList();
    return EActiveTimerReturnType::Continue;
}

void SSaveFileTable::UpdateFilesList(const FString& InDirectory, const TSet<FString>& InFiles) {
    Directory = InDirectory;
    Files = InFiles;

    Refresh();
}

void SSaveFileTable::GenerateHeader() {
    HeaderColums.Add(FHeaderColumnSettings(TEXT("SaveFileName"), FText::FromString(TEXT("File Name")), 0.25f));
    HeaderColums.Add(FHeaderColumnSettings(TEXT("SaveClass"), FText::FromString(TEXT("Class")), 0.55f));
    HeaderColums.Add(FHeaderColumnSettings(TEXT("EditButton"), FText::FromString(TEXT("")), 0.1f));
    HeaderColums.Add(FHeaderColumnSettings(TEXT("DeleteButton"), FText::FromString(TEXT("")), 0.1f));
}

void SSaveFileTable::GenerateItemsSource() {
    ItemsSource.Empty();

    for (FString& File : Files) {
        ItemsSource.Emplace(MakeShareable(new FFileListItem(Directory, File)));
    }
}

TSharedRef<ITableRow> SSaveFileTable::OnGenerateRow(TSharedPtr<FSimpleListItemBase> InItem, const TSharedRef<STableViewBase>& OwnerTable) {
    return SNew(SSaveFileEntry, InItem, OwnerTable);
}

TSharedRef<SWidget> SSaveFileEntry::GenerateWidgetForColumn(const FName& ColumnName) {
    FileListItem = static_cast<FFileListItem*>(Item.Get());

    Directory = FileListItem->Directory;
    File = FileListItem->File;

    LoadHeaderDataFromDisk();

    if (ColumnName == TEXT("SaveFileName")) {
        return SNew(SBox)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .Padding(FMargin(10.0f, 5.0f))
                [SNew(STextBlock)
                        .Font(FSaveFileEditorStyle::Get().GetFontStyle("SaveFileEditor.BoldFont"))
                        .Text(FText::FromString(File))];
    } else if (ColumnName == TEXT("SaveClass")) {
        return SNew(SBox)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .Padding(FMargin(10.0f, 5.0f))
                [SNew(STextBlock)
                        .Font(FSaveFileEditorStyle::Get().GetFontStyle("SaveFileEditor.NormalFontItalic"))
                        .Text(SaveClassText)];
    } else if (ColumnName == TEXT("EditButton")) {
        return SNew(SOverlay)
            + SOverlay::Slot()
                [SNew(SButton)
                        .OnClicked(this, &SSaveFileEntry::EditSaveFileClick)
                        .Text(LOCTEXT("EditSaveFile", "Edit"))
                        .HAlign(EHorizontalAlignment::HAlign_Center)
                        .VAlign(EVerticalAlignment::VAlign_Center)];
    } else if (ColumnName == TEXT("DeleteButton")) {
        return SNew(SOverlay)
            + SOverlay::Slot()
                [SNew(SButton)
                        .IsEnabled(this, &SSaveFileEntry::IsButtonsEnabled)
                        .OnClicked(this, &SSaveFileEntry::DeleteSaveFileClick)
                        .Text(LOCTEXT("DeleteSaveFile", "Delete"))
                        .HAlign(EHorizontalAlignment::HAlign_Center)
                        .VAlign(EVerticalAlignment::VAlign_Center)];
    }

    return SNullWidget::NullWidget;
}

bool SSaveFileEntry::IsButtonsEnabled() const {
    return !FSaveFileEditorModule::Get().IsGameStarted();
}

FReply SSaveFileEntry::EditSaveFileClick() {
    TSharedPtr<FTabManager> SaveEditorTabManager = FSaveFileEditorModule::Get().GetSaveEditorTabManager();
    if (!SaveEditorTabManager.IsValid()) {
        return FReply::Unhandled();
    }

    TSharedPtr<SDockTab> SaveEditorTab = SaveEditorTabManager->FindExistingLiveTab(SSaveFileEditorTab::GetTabName());
    if (!SaveEditorTab.IsValid()) {
        SaveEditorTab = SaveEditorTabManager->TryInvokeTab(SSaveFileEditorTab::GetTabName());
        if (!SaveEditorTab) {
            return FReply::Unhandled();
        }
    }

    SaveEditorTabManager->DrawAttention(SaveEditorTab.ToSharedRef());
    SaveEditorTab->ActivateInParent(ETabActivationCause::SetDirectly);

    TSharedPtr<SSaveFileEditorTab> SafeFileEditorTab = FSaveFileEditorModule::Get().CurrentSaveFileEditorTab.Pin();
    SafeFileEditorTab->LoadObjectFromDisk(GetFullPathFile());

    return FReply::Handled();
}

FReply SSaveFileEntry::DeleteSaveFileClick() {

    EAppReturnType::Type ReturnDialog = FMessageDialog::Open(EAppMsgType::OkCancel, EAppReturnType::Cancel, LOCTEXT("ConfirmDeleteSaveFile", "Are you sure?"));

    if (ReturnDialog != EAppReturnType::Ok) {
        return FReply::Handled();
    }

    FString FullFile = FPaths::Combine(Directory, File);
    if (FPaths::ValidatePath(FullFile) && FPaths::FileExists(FullFile)) {
        IFileManager& FileManager = IFileManager::Get();
        FileManager.Delete(*FullFile);
    }

    TWeakPtr<SSaveFileEditorTab> CurrentSaveFileEditorTab = FSaveFileEditorModule::Get().CurrentSaveFileEditorTab;
    if (CurrentSaveFileEditorTab.IsValid()) {
        if (CurrentSaveFileEditorTab.Pin()->GetCurrentFile() == FullFile) {
            CurrentSaveFileEditorTab.Pin()->ClearEditedFile();
        }
    }

    OnSaveFileDeleted.ExecuteIfBound(FullFile);

    return FReply::Handled();
}

void SSaveFileEntry::LoadHeaderDataFromDisk() {
    TArray<uint8> SaveData;
    ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();

    FString SlotName = FPaths::ConvertRelativePathToFull(FPaths::Combine(Directory, File));

    if (SaveSystem && (SlotName.Len() > 0)) {
        if (FFileHelper::LoadFileToArray(SaveData, *SlotName)) {

            FMemoryReader MemoryReader(SaveData, true);

            FSaveGameHeader SaveHeader;
            SaveHeader.Read(MemoryReader);

            SaveClassText = FText::FromString(SaveHeader.SaveGameClassName);
            return;
        }
    }

    SaveClassText = LOCTEXT("ErrorNoClassSaveFound", "Error: Incorrect .sav file");

    // Clear buffer on a failed read
    SaveData.Reset();
}

#undef LOCTEXT_NAMESPACE