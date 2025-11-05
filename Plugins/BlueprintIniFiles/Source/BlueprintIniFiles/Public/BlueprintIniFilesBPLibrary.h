// Copyright Flash 2024. All Rights Reserved. https://beacons.ai/flassh

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintIniFilesBPLibrary.generated.h"

enum class EIniFile : uint8;

UCLASS()
class UBlueprintIniFilesBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static void GetGeneralProjectSetting(const FString& Key, FString& Value);

	/** Get Project Description (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectDescription(FString& Description);

	/** Get Project ID (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectID(FString& ID);

	/** Get Project Name (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectName(FString& Name);
	
	/** Get Project Version (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectVersion(FString& Version);

	/** Get Project Company Name (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectCompanyName(FString& Name);

	/** Get Project Company Distinguished Name (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectCompanyDistinguishedName(FString& Name);

	/** Get Project Homepage (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectHomepage(FString& Homepage);

	/** Get Project Support Contact (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectSupportContact(FString& SupportContact);

	/** Get Project Copyright Notice (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectCopyrightNotice(FString& CopyrightNotice);

	/** Get Project Licensing Terms (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectLicensingTerms(FString& LicensingTerms);

	/** Get Project Privacy Policy (The one set in ProjectSettings/Project/Description) */
	UFUNCTION(BlueprintPure, Category = "Utilities|Project Settings")
	static void GetProjectPrivacyPolicy(FString& PrivacyPolicy);

	static FString GetAbsoluteIniFileName(const FString& FileName, const FString& OptionalPath);

	/**
	 * Get String Setting in Any Ini file
	 * Example: FileName = "DefaultGame.ini", SectionName = "/Script/EngineSettings.GeneralProjectSettings", SettingName = "ProjectID" to get the ProjectID.
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 * 
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsString(const FString& FileName, const FString& SectionName, const FString& SettingName, FString& Setting, const FString& OptionalPath);

	/**
	 * Set String Setting in Any Ini file
	 * Example: FileName = "MyAwesomeIniFile.ini", SectionName = "MyAwesomeSection", SettingName = "MyAwesomeSetting", SettingValue = "AwesomeValue".
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsString(const FString& FileName, const FString& SectionName, const FString& SettingName, FString SettingValue, const FString& OptionalPath);

	/**
	 * Get Bool Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsBool(const FString& FileName, const FString& SectionName, const FString& SettingName, bool& Setting, const FString& OptionalPath);

	/**
	 * Set Bool Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsBool(const FString& FileName, const FString& SectionName, const FString& SettingName, bool SettingValue, const FString& OptionalPath);

	/**
	 * Get Color Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsColor(const FString& FileName, const FString& SectionName, const FString& SettingName, FColor& Setting, const FString& OptionalPath);

	/**
	 * Set Color Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsColor(const FString& FileName, const FString& SectionName, const FString& SettingName, FColor SettingValue, const FString& OptionalPath);

	/**
	 * Get Float (Double precision) Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsDouble(const FString& FileName, const FString& SectionName, const FString& SettingName, double& Setting, const FString& OptionalPath);

	/**
	 * Set Float (Double precision) Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsDouble(const FString& FileName, const FString& SectionName, const FString& SettingName, double SettingValue, const FString& OptionalPath);

	/**
	 * Get Float (Single precision) Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsFloat(const FString& FileName, const FString& SectionName, const FString& SettingName, float& Setting, const FString& OptionalPath);

	/**
	 * Set Float (Single precision) Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsFloat(const FString& FileName, const FString& SectionName, const FString& SettingName, float SettingValue, const FString& OptionalPath);

	/**
	 * Get Int Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsInt(const FString& FileName, const FString& SectionName, const FString& SettingName, int32& Setting, const FString& OptionalPath);

	/**
	 * Set Int Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsInt(const FString& FileName, const FString& SectionName, const FString& SettingName, int32 SettingValue, const FString& OptionalPath);

	/**
	 * Get Rotator Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsRotator(const FString& FileName, const FString& SectionName, const FString& SettingName, FRotator& Setting, const FString& OptionalPath);

	/**
	 * Set Rotator Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsRotator(const FString& FileName, const FString& SectionName, const FString& SettingName, FRotator SettingValue, const FString& OptionalPath);

	/**
	 * Get Text Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsText(const FString& FileName, const FString& SectionName, const FString& SettingName, FText& Setting, const FString& OptionalPath);

	/**
	 * Set Text Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsText(const FString& FileName, const FString& SectionName, const FString& SettingName, FText SettingValue, const FString& OptionalPath);

	/**
	 * Get Vector Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsVector(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector& Setting, const FString& OptionalPath);

	/**
	 * Set Vector Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsVector(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector SettingValue, const FString& OptionalPath);

	/**
	 * Get Vector 4 Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsVector4(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector4& Setting, const FString& OptionalPath);

	/**
	 * Set Vector 4 Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsVector4(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector4 SettingValue, const FString& OptionalPath);

	/**
	 * Get Vector 2D Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static bool GetIniSettingAsVector2D(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector2D& Setting, const FString& OptionalPath);

	/**
	 * Set Vector 2D Setting in Any Ini file
	 * @param OptionalPath If empty default path (ProjectDir/Config) is used, otherwise entered path will be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void SetIniSettingAsVector2D(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector2D SettingValue, const FString& OptionalPath);

	
	static FString GetIniFileFromEnum(const EIniFile IniFile);
	
	/**
	 * Get String Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files", meta=(AdvancedDisplay = 4))
	static void GetDefaultIniSettingAsString(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FString& Setting);

	/**
	 * Set String Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files")
	static void SetDefaultIniSettingAsString(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FString SettingValue);

	/**
	 * Get Bool Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files")
	static void GetDefaultIniSettingAsBool(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, bool& Setting);

	/**
	 * Set Bool Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files")
	static void SetDefaultIniSettingAsBool(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, bool SettingValue);

	/**
	 * Get Float Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files")
	static void GetDefaultIniSettingAsFloat(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, float& Setting);

	/**
	 * Set Float Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files")
	static void SetDefaultIniSettingAsFloat(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, float SettingValue);

	/**
	 * Get Integer Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files")
	static void GetDefaultIniSettingAsInt(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, int32& Setting);

	/**
	 * Set Integer Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files")
	static void SetDefaultIniSettingAsInt(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, int32 SettingValue);

	/**
	 * Get Text Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Ini Files")
	static void GetDefaultIniSettingAsText(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FText& Setting);

	/**
	 * Set Text Setting in Defaults Ini file
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Utilities|Ini Files")
	static void SetDefaultIniSettingAsText(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FText SettingValue);
};
