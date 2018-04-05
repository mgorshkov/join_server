#pragma once

#include <array>

#include "structs.h"
#include "context.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket aSocket, std::shared_ptr<CommandExecutor> aCommandExecutor);
    ~Session();

    void Start();

private:
    void Stop();

    void DoRead();
    void Deliver(std::size_t length);

    CommandExecutor mCommandExecutor;
    tcp::socket mSocket;
    static const std::size_t BufSize = 256;
    std::array<char, BufSize> mReadMsg;
};
