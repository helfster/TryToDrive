// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "Framework/Docking/WorkspaceItem.h"
#include "LevelEditor.h"

#include "Modules/ModuleManager.h"

class IModuleEditorExtensionInterface;
class FUICommandList;

#define MODULEEXTENSION_GETTER(ModuleClass)                                                  \
    static inline ModuleClass& Get() {                                                       \
        return FModuleManager::LoadModuleChecked<ModuleClass>(ModuleClass::GetModuleName()); \
    }                                                                                        \
    static inline bool IsAvailable() {                                                       \
        return FModuleManager::Get().IsModuleLoaded(ModuleClass::GetModuleName());           \
    }

/**
 * Used by different classes to execute code on startup and shutdown of the module
 */
class IModuleListenerInterface {
public:
    IModuleListenerInterface() : ModuleOwner(nullptr) { }
    IModuleListenerInterface(IModuleEditorExtensionInterface* InModuleOwner) : ModuleOwner(InModuleOwner) { }

    IModuleEditorExtensionInterface* ModuleOwner;

    virtual void OnStartupModule() {};
    virtual void OnShutdownModule() {};
};

/**
 * Parent class for different modules, used to execute code on anything binded to the module at startup and shutdown
 */
class IModuleEditorExtensionInterface : public IModuleInterface {
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    /** ~IModuleInterface implementation */

    /**
     * Called at the beginning of startup module dispatch. Override this to register things like IModuleListenerInterface child class
     * You can for example call AddToListener(new SlateTabThings()) to add somethings to the list
     */
    virtual void AddModuleListeners() {};

    /* Used to get the current editor World reference */
    static UWorld* GetWorld();

    // Property end details in blueprint
    void RegisterDetails(FName PropertyTypeName, FOnGetDetailCustomizationInstance Delegate);
    void RegisterProperty(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance Delegate);
    void RegisterVisualPinFactory(FGraphPanelPinFactory* PinFactory);

    // Add Editor Menu extension

    /* Used to register a simple menu extension (to add an entry in the editor menu) */
    void AddMenuExtension(const FMenuExtensionDelegate& extensionDelegate, FName extensionHook,
        const TSharedPtr<FUICommandList>& CommandList = NULL, EExtensionHook::Position position = EExtensionHook::Before);

    FORCEINLINE TSharedPtr<FWorkspaceItem> GetMenuRoot() { return MenuRoot; };
    FORCEINLINE TSharedRef<FWorkspaceItem> GetMenuRootRef() { return MenuRoot.ToSharedRef(); };

    // Plugins
    FORCEINLINE TSharedPtr<FUICommandList> GetPluginCommands() { return PluginCommands; };

protected:
    /**
     * Add an object that implement IModuleListenerInterface to this module.
     * It will call Startup and Shutdown on the corresponding module's moment
     */
    FORCEINLINE void AddToListener(IModuleListenerInterface* Listener) {
        Listener->ModuleOwner = this;
        ModulesListener.Add(MakeShareable(Listener));
    }

    void UnregisterProperty(FName PropertyTypeName);
    void UnregisterDetails(FName PropertyTypeName);

private:
    TArray<TSharedRef<IModuleListenerInterface>> ModulesListener;

    TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
    TSharedPtr<FExtender> MenuExtender;

    TSharedPtr<FWorkspaceItem> MenuRoot;

    TArray<FName> RegisteredProperties;
    TArray<FName> RegisteredDetails;
    TArray<TSharedPtr<FGraphPanelPinFactory>> RegisteredPinFactory;

    TSharedPtr<FUICommandList> PluginCommands;
};
