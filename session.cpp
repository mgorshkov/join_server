#include <iostream>

#include "session.h"

using boost::asio::ip::tcp;

Session::Session(tcp::socket aSocket, std::shared_ptr<CommandExecutor> aCommandExecutor)
    : mSocket(std::move(aSocket))
    , mContext(aCommandExecutor)
{
#ifdef DEBUG_PRINT
    std::cout << "Session::Session, this==" << this << std::endl;
#endif
}

Session::~Session()
{
#ifdef DEBUG_PRINT
    std::cout << "Session::~Session, this==" << this << std::endl;
#endif
}

void Session::Start()
{
    mContext.Start();
    DoRead();
}

void Session::Stop()
{
    mSocket.close();
    mContext.Stop();
}

void Session::DoRead()
{
    auto self(shared_from_this());

    boost::asio::async_read(mSocket,
        boost::asio::buffer(mReadMsg),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
#ifdef DEBUG_PRINT
            std::cout << "Session::DoRead, this==" << this << ", ec=" << ec << ", mReadMsg=" << &mReadMsg[0] << ", length=" << length << std::endl;
#endif
            Deliver(length);

            if (!ec)
            {
                DoRead();
                DoWrite();
            }
            else
                Stop();
        });
}
  
void Session::DoWrite()
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
                Stop();
        });
}

void Session::Deliver(std::size_t length)
{
#ifdef DEBUG_PRINT
    std::cout << "Session::Deliver, this==" << this << ", mReadMsg.data()=" << mReadMsg.data() << ", mReadMsg.size()=" << mReadMsg.size() << std::endl;
#endif

    mContext.ProcessData(mReadMsg.data(), length);
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
