#include <iostream>

#include "client.h"

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_service& aIoService, tcp::resolver::iterator aEndpointIterator)
    : mIoService(aIoService)
    , mSocket(aIoService)
{
    DoConnect(aEndpointIterator);
}

void Client::DoJob()
{
    Write("0\n");
    Write("INSERT A 0 lean\n");
    Write("INSERT A 0 understand\n");
    Write("INSERT A 1 sweater\n");
    Write("INSERT A 2 frank\n");
}

void Client::Write(const std::string& aMsg)
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

void Client::Close()
{
    mIoService.post([this]() { mSocket.close(); });
}

void Client::DoConnect(tcp::resolver::iterator aEndpointIterator)
{
    boost::asio::async_connect(mSocket, aEndpointIterator,
        [this](boost::system::error_code ec, tcp::resolver::iterator)
        {
            if (ec)
                std::cout << "Connect error: " << ec.message() << std::endl;
            if (!ec)
                DoWrite();
        });
}

void Client::DoWrite()
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
