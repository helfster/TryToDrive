// Copyright Flash 2024. All Rights Reserved. https://beacons.ai/flassh

#pragma once

#include "Modules/ModuleManager.h"
#include "BlueprintIniFiles.generated.h"

class FBlueprintIniFilesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

UENUM(BlueprintType)
enum class EIniFile : uint8
{
	GEngineIni UMETA(DisplayName = "Engine"),
	GGameIni UMETA(DisplayName = "Game"),
	GInputIni UMETA(DisplayName = "Input"),
	GDeviceProfilesIni UMETA(DisplayName = "Device Profiles"),
	GGameUserSettingsIni UMETA(DisplayName = "Game User Settings"),
	GScalabilityIni UMETA(DisplayName = "Scalability"),
	GRuntimeOptionsIni UMETA(DisplayName = "Runtime Options"),
	GInstallBundleIni UMETA(DisplayName = "Install bundle"),
	GHardwareIni UMETA(DisplayName = "Hardware"),
	GGameplayTagsIni UMETA(DisplayName = "Gameplay Tags")
};
