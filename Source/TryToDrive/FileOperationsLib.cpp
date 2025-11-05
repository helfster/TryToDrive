// Fill out your copyright notice in the Description page of Project Settings.


#include "FileOperationsLib.h"
#include "Misc/FileHelper.h"
#include "FileWriteAsyncTask.h"
#include "HAL/PlatformFileManager.h"

bool UFileOperationsLib::ReadFileToByteArray(const FString& FilePath, TArray<uint8>& OutByteArray)
{
    // Проверяем существует ли файл
    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File does not exist: %s"), *FilePath);
        return false;
    }

    // Читаем файл в массив байтов
    if (FFileHelper::LoadFileToArray(OutByteArray, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully read file: %s"), *FilePath);
        return true;
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *FilePath);
    return false;
}

bool UFileOperationsLib::SaveByteArrayToFile(const FString& FilePath, const TArray<uint8>& ByteArray)
{
    // Проверяем, что массив не пустой
    if (ByteArray.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Byte array is empty!"));
        return false;
    }

    // Проверяем/создаем директорию
    FString Directory = FPaths::GetPath(FilePath);
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Directory))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*Directory);

        if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Directory))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create directory: %s"), *Directory);
            return false;
        }
    }

    // Сохраняем данные в файл
    if (FFileHelper::SaveArrayToFile(ByteArray, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("File saved successfully to: %s"), *FilePath);
        return true;
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to save file to: %s"), *FilePath);
    return false;
}