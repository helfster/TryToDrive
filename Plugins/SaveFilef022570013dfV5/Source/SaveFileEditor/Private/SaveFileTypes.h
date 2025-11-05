// Copyright Hubber Studios 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class USaveGame;

// Copied by Engine\Source\Runtime\Engine\Private\GameplayStatics.cpp (Why is not public?)
//////////////////////////////////////////////////////////////////////////
// FSaveGameHeader

static const int UE_SAVEGAME_FILE_TYPE_TAG = 0x53415647; // "SAVG"

struct FSaveGameFileVersion {
    enum Type {
        InitialVersion = 1,
        // serializing custom versions into the savegame data to handle that type of versioning
        AddedCustomVersions = 2,
        // added a new UE5 version number to FPackageFileSummary
        PackageFileSummaryVersionChange = 3,

        // -----<new versions can be added above this line>-------------------------------------------------
        VersionPlusOne,
        LatestVersion = VersionPlusOne - 1
    };
};

struct FSaveGameHeader {
    FSaveGameHeader();
    FSaveGameHeader(TSubclassOf<USaveGame> ObjectType);

    void Empty();
    bool IsEmpty() const;

    void Read(FMemoryReader& MemoryReader);
    void Write(FMemoryWriter& MemoryWriter);

    int32 FileTypeTag;
    int32 SaveGameFileVersion;
    FPackageFileVersion PackageFileUEVersion;
    FEngineVersion SavedEngineVersion;
    int32 CustomVersionFormat;
    FCustomVersionContainer CustomVersions;
    FString SaveGameClassName;
};
