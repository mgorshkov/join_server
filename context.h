#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <sstream>
#include <condition_variable>
#include <atomic>

#include "commandexecutor.h"

class Context
{
public:
    Context();
    ~Context();

    void SetExecutor(std::shared_ptr<CommandExecutor> aCommandExecutor);

    void ProcessData(const char* aData, std::size_t aSize);

    void Start();
    void Stop();

private:
    void ProcessStream(std::shared_ptr<CommandExecutor> aCommandExecutor);

    static void ThreadProc(Context* aContext, std::shared_ptr<CommandExecutor> aCommandExecutor);

    std::shared_ptr<CommandExecutor> mCommandExecutor;

    std::stringstream mStream;
    std::mutex mStreamMutex;
    std::condition_variable mCondition;
    std::atomic_bool mDone{false};
    std::atomic_bool mNotified{false};

    std::thread mThread;
};
