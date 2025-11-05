// Copyright Hubber Studios 2023. All Rights Reserved.

#include "SaveFileEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSaveFileEditorModule"

void FSaveFileEditorCommands::RegisterCommands() {
    UI_COMMAND(OpenPluginWindow, "Save Files Window", "Bring up SaveFiles window", EUserInterfaceActionType::Check, FInputChord());
    UI_COMMAND(OpenSaveManagerWindow, "View Save Files Manager", "Bring up SaveFilesManager window", EUserInterfaceActionType::Check, FInputChord());
    UI_COMMAND(OpenSaveEditorWindow, "View Save File Editor", "Bring up SaveFileEditor window", EUserInterfaceActionType::Check, FInputChord());
}

#undef LOCTEXT_NAMESPACE
