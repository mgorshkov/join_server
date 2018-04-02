#pragma once

class Server
{
public:
    Server(boost::asio::io_service& aIoService, const tcp::endpoint& aEndpoint);

private:
    void DoAccept();

    tcp::acceptor mAcceptor;
    tcp::socket mSocket;

    CommandProcessor mCommandProcessor;
};
