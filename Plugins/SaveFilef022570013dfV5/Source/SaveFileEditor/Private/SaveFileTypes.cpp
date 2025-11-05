// Copyright Hubber Studios 2023. All Rights Reserved.

#include "SaveFileTypes.h"
#include "GameFramework/SaveGame.h"

FSaveGameHeader::FSaveGameHeader()
    : FileTypeTag(0), SaveGameFileVersion(0), CustomVersionFormat(static_cast<int32>(ECustomVersionSerializationFormat::Unknown)) { }

FSaveGameHeader::FSaveGameHeader(TSubclassOf<USaveGame> ObjectType)
    : FileTypeTag(UE_SAVEGAME_FILE_TYPE_TAG), SaveGameFileVersion(FSaveGameFileVersion::LatestVersion), PackageFileUEVersion(GPackageFileUEVersion), SavedEngineVersion(FEngineVersion::Current()), CustomVersionFormat(static_cast<int32>(ECustomVersionSerializationFormat::Latest)), CustomVersions(FCurrentCustomVersions::GetAll()), SaveGameClassName(ObjectType->GetPathName()) { }

void FSaveGameHeader::Empty() {
    FileTypeTag = 0;
    SaveGameFileVersion = 0;
    PackageFileUEVersion.Reset();
    SavedEngineVersion.Empty();
    CustomVersionFormat = (int32)ECustomVersionSerializationFormat::Unknown;
    CustomVersions.Empty();
    SaveGameClassName.Empty();
}

bool FSaveGameHeader::IsEmpty() const {
    return (FileTypeTag == 0);
}

void FSaveGameHeader::Read(FMemoryReader& MemoryReader) {
    Empty();

    MemoryReader << FileTypeTag;

    if (FileTypeTag != UE_SAVEGAME_FILE_TYPE_TAG) {
        // this is an old saved game, back up the file pointer to the beginning and assume version 1
        MemoryReader.Seek(0);
        SaveGameFileVersion = FSaveGameFileVersion::InitialVersion;

        // Note for 4.8 and beyond: if you get a crash loading a pre-4.8 version of your savegame file and
        // you don't want to delete it, try uncommenting these lines and changing them to use the version
        // information from your previous build. Then load and resave your savegame file.
        // MemoryReader.SetUEVer(MyPreviousUEVersion);				// @see GPackageFileUEVersion
        // MemoryReader.SetEngineVer(MyPreviousEngineVersion);		// @see FEngineVersion::Current()
    } else {
        // Read version for this file format
        MemoryReader << SaveGameFileVersion;

        // Read engine and UE version information
        if (SaveGameFileVersion >= FSaveGameFileVersion::PackageFileSummaryVersionChange) {
            MemoryReader << PackageFileUEVersion;
        } else {
            int32 OldUe4Version;
            MemoryReader << OldUe4Version;

            PackageFileUEVersion = FPackageFileVersion::CreateUE4Version(OldUe4Version);
        }

        MemoryReader << SavedEngineVersion;

        MemoryReader.SetUEVer(PackageFileUEVersion);
        MemoryReader.SetEngineVer(SavedEngineVersion);

        if (SaveGameFileVersion >= FSaveGameFileVersion::AddedCustomVersions) {
            MemoryReader << CustomVersionFormat;

            CustomVersions.Serialize(MemoryReader, static_cast<ECustomVersionSerializationFormat::Type>(CustomVersionFormat));
            MemoryReader.SetCustomVersions(CustomVersions);
        }
    }

    // Get the class name
    MemoryReader << SaveGameClassName;
}

void FSaveGameHeader::Write(FMemoryWriter& MemoryWriter) {
    // write file type tag. identifies this file type and indicates it's using proper versioning
    // since older UE versions did not version this data.
    MemoryWriter << FileTypeTag;

    // Write version for this file format
    MemoryWriter << SaveGameFileVersion;

    // Write out engine and UE version information
    MemoryWriter << PackageFileUEVersion;
    MemoryWriter << SavedEngineVersion;

    // Write out custom version data
    MemoryWriter << CustomVersionFormat;
    CustomVersions.Serialize(MemoryWriter, static_cast<ECustomVersionSerializationFormat::Type>(CustomVersionFormat));

    // Write the class name so we know what class to load to
    MemoryWriter << SaveGameClassName;
}