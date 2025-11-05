// Copyright Hubber Studios 2023. All Rights Reserved.

#include "SSaveFileEditorTab.h"

#include "DesktopPlatformModule.h"
#include "GameFramework/SaveGame.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/GameplayStatics.h"
#include "PlatformFeatures.h"
#include "PropertyEditorDelegates.h"
#include "SKismetInspector.h"
#include "SaveGameSystem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include "SaveFileEditor.h"
#include "SaveFileEditorStyle.h"
#include "SaveFileTypes.h"

#define LOCTEXT_NAMESPACE "FSaveFileEditorModule"

void SSaveFileEditorTab::Construct(const FArguments& InArgs) {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs Args;
    Args.bHideSelectionTip = true;
    Args.bForceHiddenPropertyVisibility = true;
    Args.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Show;
    PropertyWidget = PropertyModule.CreateDetailView(Args);

    PropertyWidget->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &SSaveFileEditorTab::IsObjectEditable));

    this->ChildSlot
        [SNew(SVerticalBox)
            + SVerticalBox::Slot()
                  .AutoHeight()
                  .Padding(5.0f)
                  .HAlign(EHorizontalAlignment::HAlign_Fill)
                  .VAlign(EVerticalAlignment::VAlign_Fill)
                      [SNew(SOverlay)
                          + SOverlay::Slot()
                                .HAlign(HAlign_Fill)
                                .VAlign(VAlign_Fill)
                                    [SAssignNew(FileNameText, STextBlock)
                                            .Text(LOCTEXT("SaveFileEditorNoFileName", "No file loaded."))]]
            + SVerticalBox::Slot()
                  .FillHeight(1.0f)
                  .Padding(0.0f, 0.0f, 0.0f, 5.0f)
                  .HAlign(EHorizontalAlignment::HAlign_Fill)
                  .VAlign(EVerticalAlignment::VAlign_Fill)
                      [SNew(SOverlay)
                          + SOverlay::Slot()
                                .HAlign(HAlign_Fill)
                                .VAlign(VAlign_Fill)
                                    [PropertyWidget.ToSharedRef()]]];

    ObjectEditHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SSaveFileEditorTab::OnObjectEdit);

    PostPIEStartedHandle = FEditorDelegates::PostPIEStarted.AddSP(this, &SSaveFileEditorTab::OnPieWorldStarted);
    EndPIEHandle = FEditorDelegates::EndPIE.AddSP(this, &SSaveFileEditorTab::OnPieWorldStopped);

    PausePIEHandle = FEditorDelegates::PausePIE.AddSP(this, &SSaveFileEditorTab::OnPieWorldStopped);
    ResumePIEHandle = FEditorDelegates::ResumePIE.AddSP(this, &SSaveFileEditorTab::OnPieWorldStarted);

    bIsEditable = !FSaveFileEditorModule::Get().IsGameStarted();
}

SSaveFileEditorTab::~SSaveFileEditorTab() {
    FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(ObjectEditHandle);

    FEditorDelegates::PostPIEStarted.Remove(PostPIEStartedHandle);
    FEditorDelegates::PostPIEStarted.Remove(EndPIEHandle);
    FEditorDelegates::PostPIEStarted.Remove(PausePIEHandle);
    FEditorDelegates::PostPIEStarted.Remove(ResumePIEHandle);

    ClearEditedObject();
    FSaveFileEditorModule::Get().CurrentSaveFileEditorTab.Reset();
}

TSharedRef<class SDockTab> SSaveFileEditorTab::OnSpawnContentTab(const FSpawnTabArgs& SpawnTabArgs) {
    TSharedPtr<SSaveFileEditorTab> SaveFileEditor;

    const TSharedRef<SDockTab> Tab = SNew(SDockTab)
                                         .TabRole(ETabRole::PanelTab)
                                         .Label(LOCTEXT("SaveFileEditorTabTitle", "Save File Editor"))
                                             [SNew(SBorder)
                                                     .BorderImage(FAppStyle::GetBrush("Docking.Tab.ContentAreaBrush"))
                                                     .BorderBackgroundColor(FSlateColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.f)))
                                                         [SAssignNew(SaveFileEditor, SSaveFileEditorTab)]];

    FSaveFileEditorModule::Get().CurrentSaveFileEditorTab = SaveFileEditor;

    return Tab;
}

void SSaveFileEditorTab::LoadObjectFromDisk(FString InFile) {
    File = InFile;
    TArray<uint8> SaveData;
    ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();

    FString SlotName = File;

    if (SaveSystem && (SlotName.Len() > 0)) {
        if (FFileHelper::LoadFileToArray(SaveData, *SlotName)) {

            FMemoryReader MemoryReader(SaveData, true);

            FSaveGameHeader SaveHeader;
            SaveHeader.Read(MemoryReader);

            // Try and find it, and failing that, load it
            UClass* SaveGameClass = UClass::TryFindTypeSlow<UClass>(SaveHeader.SaveGameClassName);
            if (SaveGameClass == nullptr) {
                SaveGameClass = LoadObject<UClass>(nullptr, *SaveHeader.SaveGameClassName);
            }

            // If we have a class, try and load it.
            if (SaveGameClass != nullptr) {
                if (!IsValid(SaveGameObject) || SaveGameObject->GetClass() != SaveGameClass) {
                    ClearEditedObject();
                    SaveGameObject = NewObject<USaveGame>(GetTransientPackage(), SaveGameClass, NAME_None, EObjectFlags::RF_MarkAsRootSet);
                }

                FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
                SaveGameObject->Serialize(Ar);

                PropertyWidget->SetObject(SaveGameObject);

                FFileManagerGeneric fm;
                FileTimeStamp = fm.GetTimeStamp(*File);

                FileNameText->SetText(FText::FromString(File));

                return;
            }
        }
    }
    FileNameText->SetText(LOCTEXT("SaveFileEditorNoFileName", "No file loaded."));
    File = FString();
}

void SSaveFileEditorTab::ClearEditedFile() {
    File = FString();
    FileNameText->SetText(LOCTEXT("SaveFileEditorNoFileName", "No file loaded."));
    ClearEditedObject();
}

void SSaveFileEditorTab::ClearEditedObject() {
    PropertyWidget->SetObject(nullptr, true);
    if (IsValid(SaveGameObject)) {
        SaveGameObject->RemoveFromRoot();
        SaveGameObject->MarkAsGarbage();
    }
    SaveGameObject = nullptr;
}

void SSaveFileEditorTab::OnPieWorldStarted(bool bIsSimulated) {
    TSharedPtr<FTabManager> TabManagerPtr = FSaveFileEditorModule::Get().GetSaveEditorTabManager();
    if (FSaveFileEditorModule::IsTabVisible(TabManagerPtr, GetTabName()) && PropertyWidget.IsValid()) {
        bIsEditable = false;
    }
}

void SSaveFileEditorTab::OnPieWorldStopped(bool bIsSimulated) {
    TSharedPtr<FTabManager> TabManagerPtr = FSaveFileEditorModule::Get().GetSaveEditorTabManager();
    if (FSaveFileEditorModule::IsTabVisible(TabManagerPtr, GetTabName()) && PropertyWidget.IsValid()) {
        bIsEditable = true;
    }
}

void SSaveFileEditorTab::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) {
    if (!File.IsEmpty()) {
        FFileManagerGeneric fm;

        if (!fm.FileExists(*File)) {
            ClearEditedFile();
        }

        FDateTime NewTimeStamp = fm.GetTimeStamp(*File);

        if (FileTimeStamp != NewTimeStamp) {
            LoadObjectFromDisk(File);
        }
    }
}

void SSaveFileEditorTab::OnObjectEdit(UObject* Obj, FPropertyChangedEvent& Property) {
    if (Obj != SaveGameObject) {
        return;
    }

    TArray<uint8> SaveData;
    if (!UGameplayStatics::SaveGameToMemory(SaveGameObject, SaveData)) {
        return;
    }

    ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();
    if (!SaveSystem || SaveData.Num() == 0) {
        return;
    }

    FFileHelper::SaveArrayToFile(SaveData, *File);

    FFileManagerGeneric fm;
    FileTimeStamp = fm.GetTimeStamp(*File);
}

#undef LOCTEXT_NAMESPACE