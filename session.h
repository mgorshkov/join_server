#pragma once

#include <array>
#include <boost/asio.hpp>
#include <memory>

#include "structs.h"
#include "context.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket aSocket,
        std::shared_ptr<CommandExecutor> aCommandExecutor,
        boost::asio::io_service& aIoService);
    ~Session();

    void Start();

private:
    void Stop();

    void DoProcessCommand();
    bool GetWriteQueue();
    void DoWrite();
    void Deliver(std::size_t length);

    std::shared_ptr<CommandExecutor> mCommandExecutor;
    boost::asio::io_service& mIoService;
    Context mContext;
    boost::asio::ip::tcp::socket mSocket;
    static const std::size_t BufSize = 1;
    std::array<char, BufSize> mReadMsg;
    std::deque<std::string> mWriteMsgs;
};
