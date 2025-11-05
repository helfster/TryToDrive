// Copyright Hubber Studios 2023. All Rights Reserved.

#include "HubberUtils/Module/IModuleExtensionInterface.h"

void IModuleEditorExtensionInterface::StartupModule() {
    if (!IsRunningCommandlet()) {
        MenuRoot = FWorkspaceItem::NewGroup(FText::FromString("Menu Root"));

        FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
        LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
        MenuExtender = MakeShareable(new FExtender);
        LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);

        // Create plugin commands
        PluginCommands = MakeShareable(new FUICommandList);

        AddModuleListeners();
        for (TSharedRef<IModuleListenerInterface> ModuleListener : ModulesListener) {
            ModuleListener->OnStartupModule();
        }
    }
}

void IModuleEditorExtensionInterface::ShutdownModule() {
    for (TSharedRef<IModuleListenerInterface> ModuleListener : ModulesListener) {
        ModuleListener->OnShutdownModule();
    }
    for (FName& Name : RegisteredProperties) {
        UnregisterProperty(Name);
    }
    for (FName& Name : RegisteredDetails) {
        UnregisterDetails(Name);
    }
    for (TSharedPtr<FGraphPanelPinFactory> PinFactory : RegisteredPinFactory) {
        FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
    }
}

UWorld* IModuleEditorExtensionInterface::GetWorld() {
    UWorld* World = nullptr;

    const TIndirectArray<FWorldContext>& WorldList = GEngine->GetWorldContexts();

    TArray<FWorldContext> NewWorldList;

    for (auto& Item : WorldList) {
        if (Item.WorldType == EWorldType::Type::PIE || Item.WorldType == EWorldType::Type::Game) {
            NewWorldList.Add(Item);
            return Item.World();
        }
    }

    return nullptr;
}

void IModuleEditorExtensionInterface::AddMenuExtension(const FMenuExtensionDelegate& extensionDelegate, FName extensionHook, const TSharedPtr<FUICommandList>& CommandList, EExtensionHook::Position position) {
    MenuExtender->AddMenuExtension(extensionHook, position, CommandList, extensionDelegate);
}

void IModuleEditorExtensionInterface::RegisterDetails(FName PropertyTypeName, FOnGetDetailCustomizationInstance Delegate) {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
    PropertyModule.RegisterCustomClassLayout(PropertyTypeName, Delegate);
    RegisteredDetails.Emplace(PropertyTypeName);
    PropertyModule.NotifyCustomizationModuleChanged();
}

void IModuleEditorExtensionInterface::RegisterProperty(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance Delegate) {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
    PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, Delegate);
    RegisteredProperties.Emplace(PropertyTypeName);
    PropertyModule.NotifyCustomizationModuleChanged();
}

void IModuleEditorExtensionInterface::RegisterVisualPinFactory(FGraphPanelPinFactory* PinFactory) {
    FEdGraphUtilities::RegisterVisualPinFactory(MakeShareable(PinFactory));
    RegisteredPinFactory.Emplace(PinFactory);
}

void IModuleEditorExtensionInterface::UnregisterProperty(FName PropertyTypeName) {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
    PropertyModule.UnregisterCustomPropertyTypeLayout(PropertyTypeName);
    PropertyModule.NotifyCustomizationModuleChanged();
}

void IModuleEditorExtensionInterface::UnregisterDetails(FName PropertyTypeName) {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
    PropertyModule.UnregisterCustomClassLayout(PropertyTypeName);
    PropertyModule.NotifyCustomizationModuleChanged();
}
