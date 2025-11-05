// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileOperationsLib.generated.h"

/**
 * 
 */
UCLASS()
class TRYTODRIVE_API UFileOperationsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:
	UFUNCTION(BlueprintCallable, Category = "File Utilities", meta = (DisplayName = "Read File to Byte Array"))
	static bool ReadFileToByteArray(const FString& FilePath, TArray<uint8>& OutByteArray);

	public:
	UFUNCTION(BlueprintCallable, Category = "File Utilities", meta = (DisplayName = "Save Byte Array to File"))
	static bool SaveByteArrayToFile(const FString& FilePath, const TArray<uint8>& ByteArray);
};
