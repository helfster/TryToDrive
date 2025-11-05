// Copyright Flash 2024. All Rights Reserved. https://beacons.ai/flassh

#include "BlueprintIniFilesBPLibrary.h"

#include "BlueprintIniFiles.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/ConfigCacheIni.h"

void UBlueprintIniFilesBPLibrary::GetGeneralProjectSetting(const FString& Key, FString& Value)
{
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), *Key, Value, GGameIni);
}

void UBlueprintIniFilesBPLibrary::GetProjectDescription(FString& Description)
{
	GetGeneralProjectSetting("Description", Description);
}

void UBlueprintIniFilesBPLibrary::GetProjectID(FString& ID)
{
	GetGeneralProjectSetting("ProjectID", ID);
}

void UBlueprintIniFilesBPLibrary::GetProjectName(FString& Name)
{
	GetGeneralProjectSetting("ProjectName", Name);
}

void UBlueprintIniFilesBPLibrary::GetProjectVersion(FString& Version)
{
	GetGeneralProjectSetting("ProjectVersion", Version);
}

void UBlueprintIniFilesBPLibrary::GetProjectCompanyName(FString& Name)
{
	GetGeneralProjectSetting("CompanyName", Name);
}

void UBlueprintIniFilesBPLibrary::GetProjectCompanyDistinguishedName(FString& Name)
{
	GetGeneralProjectSetting("CompanyDistinguishedName", Name);
}

void UBlueprintIniFilesBPLibrary::GetProjectHomepage(FString& Homepage)
{
	GetGeneralProjectSetting("Homepage", Homepage);
}

void UBlueprintIniFilesBPLibrary::GetProjectSupportContact(FString& SupportContact)
{
	GetGeneralProjectSetting("SupportContact", SupportContact);
}

void UBlueprintIniFilesBPLibrary::GetProjectCopyrightNotice(FString& CopyrightNotice)
{
	GetGeneralProjectSetting("CopyrightNotice", CopyrightNotice);
}

void UBlueprintIniFilesBPLibrary::GetProjectLicensingTerms(FString& LicensingTerms)
{
	GetGeneralProjectSetting("LicensingTerms", LicensingTerms);
}

void UBlueprintIniFilesBPLibrary::GetProjectPrivacyPolicy(FString& PrivacyPolicy)
{
	GetGeneralProjectSetting("PrivacyPolicy", PrivacyPolicy);
}

FString UBlueprintIniFilesBPLibrary::GetAbsoluteIniFileName(const FString& FileName, const FString& OptionalPath)
{
	const FString AbsoluteFileName = OptionalPath.IsEmpty() ? FPaths::ProjectDir() + "Config/" + FileName : FPaths::Combine(OptionalPath, FileName);
	return FConfigCacheIni::NormalizeConfigIniPath(AbsoluteFileName); // Normalize path
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsString(const FString& FileName, const FString& SectionName, const FString& SettingName, FString& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetString(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = "";
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsString(const FString& FileName, const FString& SectionName, const FString& SettingName, FString SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetString(*SectionName, *SettingName, *SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsBool(const FString& FileName, const FString& SectionName, const FString& SettingName, bool& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetBool(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = false;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsBool(const FString& FileName, const FString& SectionName, const FString& SettingName, bool SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetBool(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsColor(const FString& FileName, const FString& SectionName, const FString& SettingName, FColor& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetColor(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = FColor::Transparent;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsColor(const FString& FileName, const FString& SectionName, const FString& SettingName, FColor SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetColor(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsDouble(const FString& FileName, const FString& SectionName, const FString& SettingName, double& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetDouble(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = 0.0f;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsDouble(const FString& FileName, const FString& SectionName, const FString& SettingName, double SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetDouble(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsFloat(const FString& FileName, const FString& SectionName, const FString& SettingName, float& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetFloat(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = -1.0f;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsFloat(const FString& FileName, const FString& SectionName, const FString& SettingName, float SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetFloat(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsInt(const FString& FileName, const FString& SectionName, const FString& SettingName, int32& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetInt(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = -1;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsInt(const FString& FileName, const FString& SectionName, const FString& SettingName, int32 SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetInt(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsRotator(const FString& FileName, const FString& SectionName, const FString& SettingName, FRotator& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetRotator(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = FRotator::ZeroRotator;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsRotator(const FString& FileName, const FString& SectionName, const FString& SettingName, FRotator SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetRotator(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsText(const FString& FileName, const FString& SectionName, const FString& SettingName, FText& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetText(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = FText::FromString("");
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsText(const FString& FileName, const FString& SectionName, const FString& SettingName, FText SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetText(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsVector(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetVector(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = FVector::ZeroVector;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsVector(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetVector(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsVector4(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector4& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetVector4(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = FVector4(0.0f, 0.0f, 0.0f, 0.0f);
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsVector4(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector4 SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetVector4(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

bool UBlueprintIniFilesBPLibrary::GetIniSettingAsVector2D(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector2D& Setting, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		
		if(GConfig->GetVector2D(*SectionName, *SettingName, Setting, AbsoluteFileName))
		{
			return true;
		}
	}

	Setting = FVector2D::ZeroVector;
	return false;
}

void UBlueprintIniFilesBPLibrary::SetIniSettingAsVector2D(const FString& FileName, const FString& SectionName, const FString& SettingName, FVector2D SettingValue, const FString& OptionalPath)
{
	const FString AbsoluteFileName = GetAbsoluteIniFileName(FileName, OptionalPath);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFileName);
	}

	GConfig->LoadFile(AbsoluteFileName);
	GConfig->SetVector2D(*SectionName, *SettingName, SettingValue, AbsoluteFileName);
	GConfig->Flush(false, AbsoluteFileName); // Save File
}

FString UBlueprintIniFilesBPLibrary::GetIniFileFromEnum(const EIniFile IniFile)
{
	switch(IniFile)
	{
	case EIniFile::GEngineIni :
		return GEngineIni;
	case EIniFile::GGameIni :
		return GGameIni;
	case EIniFile::GInputIni :
		return GInputIni;
	case EIniFile::GDeviceProfilesIni :
		return GDeviceProfilesIni;
	case EIniFile::GGameUserSettingsIni :
		return GGameUserSettingsIni;
	case EIniFile::GScalabilityIni :
		return GScalabilityIni;
	case EIniFile::GRuntimeOptionsIni :
		return GRuntimeOptionsIni;
	case EIniFile::GInstallBundleIni :
		return GInstallBundleIni;
	case EIniFile::GHardwareIni :
		return GHardwareIni;
	case EIniFile::GGameplayTagsIni :
		return GGameplayTagsIni;
	}

	return "";
}

void UBlueprintIniFilesBPLibrary::GetDefaultIniSettingAsString(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FString& Setting)
{
	const FString AbsoluteFileName = GetIniFileFromEnum(IniFile);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		Setting = GConfig->GetStringOrDefault(*SectionName, *SettingName, Setting, AbsoluteFileName);
	}
}

void UBlueprintIniFilesBPLibrary::SetDefaultIniSettingAsString(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FString SettingValue)
{
	const FString FileName = GetIniFileFromEnum(IniFile);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *FileName);
	}

	GConfig->LoadFile(FileName);
	GConfig->SetString(*SectionName, *SettingName, *SettingValue, FileName);
	GConfig->Flush(false, FileName); // Save File
}

void UBlueprintIniFilesBPLibrary::GetDefaultIniSettingAsBool(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, bool& Setting)
{
	const FString AbsoluteFileName = GetIniFileFromEnum(IniFile);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		Setting = GConfig->GetBoolOrDefault(*SectionName, *SettingName, Setting, AbsoluteFileName);
	}
}

void UBlueprintIniFilesBPLibrary::SetDefaultIniSettingAsBool(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, bool SettingValue)
{
	const FString FileName = GetIniFileFromEnum(IniFile);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *FileName);
	}

	GConfig->LoadFile(FileName);
	GConfig->SetBool(*SectionName, *SettingName, SettingValue, FileName);
	GConfig->Flush(false, FileName); // Save File
}

void UBlueprintIniFilesBPLibrary::GetDefaultIniSettingAsFloat(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, float& Setting)
{
	const FString AbsoluteFileName = GetIniFileFromEnum(IniFile);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		Setting = GConfig->GetFloatOrDefault(*SectionName, *SettingName, Setting, AbsoluteFileName);
	}
}

void UBlueprintIniFilesBPLibrary::SetDefaultIniSettingAsFloat(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, float SettingValue)
{
	const FString FileName = GetIniFileFromEnum(IniFile);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *FileName);
	}

	GConfig->LoadFile(FileName);
	GConfig->SetFloat(*SectionName, *SettingName, SettingValue, FileName);
	GConfig->Flush(false, FileName); // Save File
}

void UBlueprintIniFilesBPLibrary::GetDefaultIniSettingAsInt(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, int32& Setting)
{
	const FString AbsoluteFileName = GetIniFileFromEnum(IniFile);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		Setting = GConfig->GetIntOrDefault(*SectionName, *SettingName, Setting, AbsoluteFileName);
	}
}

void UBlueprintIniFilesBPLibrary::SetDefaultIniSettingAsInt(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, int32 SettingValue)
{
	const FString FileName = GetIniFileFromEnum(IniFile);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *FileName);
	}

	GConfig->LoadFile(FileName);
	GConfig->SetInt(*SectionName, *SettingName, SettingValue, FileName);
	GConfig->Flush(false, FileName); // Save File
}

void UBlueprintIniFilesBPLibrary::GetDefaultIniSettingAsText(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FText& Setting)
{
	const FString AbsoluteFileName = GetIniFileFromEnum(IniFile);
	
	if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFileName))
	{
		GConfig->LoadFile(AbsoluteFileName);
		Setting = GConfig->GetTextOrDefault(*SectionName, *SettingName, Setting, AbsoluteFileName);
	}
}

void UBlueprintIniFilesBPLibrary::SetDefaultIniSettingAsText(const EIniFile IniFile, const FString& SectionName, const FString& SettingName, FText SettingValue)
{
	const FString FileName = GetIniFileFromEnum(IniFile);

	// Create file if it does not exist
	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FileName))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *FileName);
	}

	GConfig->LoadFile(FileName);
	GConfig->SetText(*SectionName, *SettingName, SettingValue, FileName);
	GConfig->Flush(false, FileName); // Save File
}