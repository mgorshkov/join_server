#pragma once

#include <boost/asio.hpp>

#include "tablestorage.h"
#include "commandexecutor.h"

class Server
{
public:
    Server(boost::asio::io_service& aIoService, const boost::asio::ip::tcp::endpoint& aEndpoint);

private:
    void DoAccept();

    tcp::acceptor mAcceptor;
    tcp::socket mSocket;

    TableManager mTableManager;
    std::shared_ptr<CommandExecutor> mCommandExecutor;
};
