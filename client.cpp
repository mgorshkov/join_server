#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "structs.h"

using boost::asio::ip::tcp;

class Client
{
public:
    Client(boost::asio::io_service& aIoService, tcp::resolver::iterator aEndpointIterator)
        : mIoService(aIoService)
        , mSocket(aIoService)
    {
        DoConnect(aEndpointIterator);
    }

    void write(const std::string& aMsg)
    {
        mIoService.post(
            [this, aMsg]()
            {
                bool writeInProgress = !mWriteMsgs.empty();
                mWriteMsgs.push_back(aMsg);
                if (!writeInProgress)
                {
                    DoWrite();
                }
            });
    }

    void close()
    {
        mIoService.post([this]() { mSocket.close(); });
    }

private:
    void DoConnect(tcp::resolver::iterator aEndpointIterator)
    {
        boost::asio::async_connect(mSocket, aEndpointIterator,
        [this](boost::system::error_code ec, tcp::resolver::iterator)
        {
          if (!ec)
          {
            DoWrite();
          }
        });
    }

    void DoWrite()
    {
        boost::asio::async_write(mSocket,
            boost::asio::buffer(mWriteMsgs.front().c_str(),
                mWriteMsgs.front().size()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                mWriteMsgs.pop_front();
                if (!mWriteMsgs.empty())
                    DoWrite();
            }
            else
                mSocket.close();
        });
    }

private:
    boost::asio::io_service& mIoService;
    tcp::socket mSocket;
    std::deque<std::string> mWriteMsgs;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: join_client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_service ioService;

        tcp::resolver resolver(ioService);
        auto host = argv[1];
        int port = std::atoi(argv[2]);
        if (port == 0)
        {
            std::cerr << "Incorrect port:" << port << std::endl;
            return 1;
        }
        auto endpointIterator = resolver.resolve({ host, argv[2] });
        Client client(ioService, endpointIterator);

        std::thread t([&ioService]()
            {
                ioService.run();
            });

        client.write("INSERT A 0 lean\n");
        client.write("INSERT A 0 understand\n");
        client.write("INSERT A 1 sweater\n");
        client.write("INSERT A 2 frank\n");

        client.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
