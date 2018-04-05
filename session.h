#pragma once

#include <array>
#include <boost/asio.hpp>

#include "structs.h"
#include "context.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket aSocket, CommandExecutor& aCommandExecutor);
    ~Session();

    void Start();

private:
    void Stop();

    void DoRead();
    void Deliver(std::size_t length);

    CommandExecutor& mCommandExecutor;
    Context mContext;
    boost::asio::ip::tcp::socket mSocket;
    static const std::size_t BufSize = 256;
    std::array<char, BufSize> mReadMsg;
};