#pragma once

#include "CoreMinimal.h"
#include "HAL/Thread.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Misc/FileHelper.h"

class FFileWriteAsyncTask : public FRunnable
{
public:
    FFileWriteAsyncTask(const FString& InFilePath, const TArray<uint8>& InByteArray, TFunction<void(bool)> InCallback)
        : FilePath(InFilePath)
        , ByteArray(InByteArray)
        , Callback(InCallback)
        , bSuccess(false)
    {
        Thread = FRunnableThread::Create(this, TEXT("FileWriteAsyncTask"));
    }

    virtual ~FFileWriteAsyncTask()
    {
        if (Thread)
        {
            Thread->Kill();
            delete Thread;
        }
    }

    // FRunnable interface
    virtual uint32 Run() override
    {
        bSuccess = FFileHelper::SaveArrayToFile(ByteArray, *FilePath);
        return 0;
    }

    virtual void Exit() override
    {
        // Вызываем callback в GameThread после завершения
        if (Callback)
        {
            AsyncTask(ENamedThreads::GameThread, [this]()
                {
                    Callback(bSuccess);
                });
        }
    }

private:
    FString FilePath;
    TArray<uint8> ByteArray;
    TFunction<void(bool)> Callback;
    bool bSuccess;
    FRunnableThread* Thread;
};