// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SaveFileEditorStyle.h"

class FSaveFileEditorCommands : public TCommands<FSaveFileEditorCommands> {
public:
    FSaveFileEditorCommands()
        : TCommands<FSaveFileEditorCommands>(TEXT("SaveFileEditor"), NSLOCTEXT("Contexts", "SaveFileEditor", "SaveFileEditor Plugin"), NAME_None, FSaveFileEditorStyle::GetStyleSetName()) {
    }

    // TCommands<> interface
    virtual void RegisterCommands() override;

public:
    TSharedPtr<FUICommandInfo> OpenPluginWindow;
    TSharedPtr<FUICommandInfo> OpenSaveManagerWindow;
    TSharedPtr<FUICommandInfo> OpenSaveEditorWindow;
};