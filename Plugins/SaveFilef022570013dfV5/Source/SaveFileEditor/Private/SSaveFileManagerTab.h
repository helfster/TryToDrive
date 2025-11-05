// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/SCompoundWidget.h"

#include "HubberUtils/Slate/SSimpleList.h"

class SWrapBox;

DECLARE_DELEGATE_OneParam(FOnSaveFileDeleted, FString /*FileName*/);

class UAchievementSubsystem;
class SSaveFileTable;

struct FFileListItem : public FSimpleListItemBase {
public:
    FFileListItem(FString InDirectory, FString InFile) : Directory(InDirectory), File(InFile) { }

    FString Directory;
    FString File;
};

class SSaveFileManagerTab : public SCompoundWidget {

    SLATE_BEGIN_ARGS(SSaveFileManagerTab) { }
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    static FName GetTabName() { return FName(TEXT("SaveFileEditorManager")); }
    static TSharedRef<class SDockTab> OnSpawnContentTab(const class FSpawnTabArgs& SpawnTabArgs);

    FString GetCurrentSaveDirectory() { return CurrentSaveDirectory; }
    TSet<FString> GetSaveFiles() { return SaveFiles; };

private:
    FString CurrentSaveDirectory;
    TSet<FString> SaveFiles;

    TSharedPtr<STextBlock> SaveDirectoryTextBlock;
    TSharedPtr<SSaveFileTable> FilesListContainer;

    void UpdateSaveFileList();

    EActiveTimerReturnType TimerUpdate(double InCurrentTime, float DeltaTime);

    FReply OpenSaveDirectoryClick();
    FReply ChangeSaveDirectoryClick();
    FReply ResetsSaveDirectoryClick();
    FReply DeleteAllSaveFilesClick();

    void SaveFileDeleted(FString File);
};

class SSaveFileTable : public SSimpleListBase {
public:
    void UpdateFilesList(const FString& InDirectory, const TSet<FString>& InFiles);

protected:
    virtual void Initialize() override { }

    virtual void GenerateHeader() override;

    virtual void GenerateItemsSource() override;

    virtual TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FSimpleListItemBase> InItem, const TSharedRef<STableViewBase>& OwnerTable) override;

private:
    FString Directory;
    TSet<FString> Files;
};

class SSaveFileEntry : public SSimpleListItemBase {

public:
    virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

    FString GetFullPathFile() { return FPaths::ConvertRelativePathToFull(FPaths::Combine(Directory, File)); }

    bool IsButtonsEnabled() const;

private:
    FString Directory;
    FString File;
    FText SaveClassText;

    FFileListItem* FileListItem;

    FOnSaveFileDeleted OnSaveFileDeleted;

    FReply EditSaveFileClick();
    FReply DeleteSaveFileClick();

    void LoadHeaderDataFromDisk();
};