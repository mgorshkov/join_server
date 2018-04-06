#include <iostream>

#include "server.h"
#include "session.h"

Server::Server(boost::asio::io_service& aIoService, const boost::asio::ip::tcp::endpoint& aEndPoint)
    : mAcceptor(aIoService)
    , mSocket(aIoService)
    , mCommandExecutor(&mTableManager)
{
    DoAccept();
}

void Server::DoAccept()
{
    mAcceptor.async_accept(mSocket,
        [this](boost::system::error_code ec)
        {
            if (!ec)
                std::make_shared<Session>(std::move(mSocket), mCommandExecutor)->Start();

            DoAccept();
        });
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: join_server <port>" << std::endl;
            return 1;
        }

        boost::asio::io_service ioService;

        int port = std::atoi(argv[1]);
        if (port == 0)
        {
            std::cerr << "Incorrect port:" << argv[1] << std::endl;
            return 1;
        }

        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        Server server(ioService, endpoint);

        ioService.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
