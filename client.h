#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "structs.h"

using boost::asio::ip::tcp;

class Client
{
public:
    Client(boost::asio::io_service& aIoService, tcp::resolver::iterator aEndpointIterator);

    void DoJob();

    void Close();

private:
    void Write(const std::string& aMsg);

    void DoConnect(tcp::resolver::iterator aEndpointIterator);

    void DoWrite();

private:
    boost::asio::io_service& mIoService;
    tcp::socket mSocket;
    std::deque<std::string> mWriteMsgs;
};

