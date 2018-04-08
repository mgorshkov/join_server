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
            GetWriteQueue();
            if (!mWriteMsgs.empty())
                DoWrite();
    
            if (!ec)
                DoRead();
            else
                Stop();
        });
}

void Session::GetWriteQueue()
{
    auto statuses = mContext.GetOutboundQueue();
    if (statuses.empty())
        return;
    std::stringstream str;
    for (const auto& status : statuses)
        str << status;
//#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue, this==" << this << ", str=" << str.str() << std::endl;
//#endif
    mWriteMsgs.push_back(str.str());
}
  
void Session::DoWrite()
{
    boost::asio::async_write(mSocket,
        boost::asio::buffer(mWriteMsgs.front().c_str(),
            mWriteMsgs.front().size()),
    [this](boost::system::error_code ec, std::size_t /*length*/)
       {
 //#ifdef DEBUG_PRINT
           std::cout << "Session::DoWrite, this==" << this << ", ec=" << ec << std::endl;
//#endif
           if (!ec)
           {
               mWriteMsgs.pop_front();
 //#ifdef DEBUG_PRINT
           std::cout << "Session::DoWrite 2, this==" << this << ", ec=" << ec << std::endl;
//#endif
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

    std::string aMsg = "test";
    boost::asio::write(mSocket, boost::asio::buffer(aMsg.c_str(), aMsg.length()));

    std::cout << "Session::Deliver write, this==" << this << ", mReadMsg.data()=" << mReadMsg.data() << ", mReadMsg.size()=" << mReadMsg.size() << std::endl;

    mWriteMsgs.push_back("test");
//DoWrite();
}
