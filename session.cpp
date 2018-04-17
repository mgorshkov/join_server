#include <iostream>

#include "session.h"

using boost::asio::ip::tcp;

Session::Session(tcp::socket aSocket, std::shared_ptr<CommandExecutor> aCommandExecutor, boost::asio::io_service& aIoService)
    : mSocket(std::move(aSocket))
    , mContext(aCommandExecutor)
    , mIoService(aIoService)
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
    DoProcessCommand();
}

void Session::Stop()
{
#ifdef DEBUG_PRINT
    std::cout << "Session::Stop 1, this==" << this << std::endl;
#endif
    mSocket.close();
    mContext.Stop();
#ifdef DEBUG_PRINT
    std::cout << "Session::Stop 2, this==" << this << std::endl;
#endif
}

void Session::DoProcessCommand()
{
    auto self(shared_from_this());

#ifdef DEBUG_PRINT
    std::cout << "Session::DoProcessCommand 1, this==" << this << std::endl;
#endif

    boost::asio::deadline_timer timer(mIoService);
    const boost::posix_time::time_duration timeout = boost::posix_time::milliseconds{100};
    timer.expires_from_now(timeout);
    timer.async_wait(
        [this, self](boost::system::error_code ec)
        {
             if (GetWriteQueue())
                 DoWrite(); 
        });
    boost::asio::async_read(mSocket,
        boost::asio::buffer(mReadMsg),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
#ifdef DEBUG_PRINT
            std::cout << "Session::DoProcessCommand 2, this==" << this << ", ec=" << ec << ", mReadMsg=" << std::string(&mReadMsg[0], length) << ", length=" << length << std::endl;
#endif
            if (!ec)
            {
                Deliver(length);
                DoProcessCommand();
            }
            else
                Stop();
        });
}

bool Session::GetWriteQueue()
{
//#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue 1, this==" << this << std::endl;
//#endif
    auto statuses = mContext.GetOutboundQueue();
    if (statuses.empty())
        return false;
    std::stringstream str;
    for (const auto& status : statuses)
        str << status;
//#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue 2, this==" << this << ", str=" << str.str() << std::endl;
//#endif
    mWriteMsgs.push_back(str.str());
    return true;
}
  
void Session::DoWrite()
{
#ifdef DEBUG_PRINT
    std::cout << "Session::DoWrite 1, this==" << this << std::endl;
#endif
    auto self(shared_from_this());

    boost::asio::async_write(mSocket,
        boost::asio::buffer(mWriteMsgs.front().c_str(),
            mWriteMsgs.front().size()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
#ifdef DEBUG_PRINT
           std::cout << "Session::DoWrite 2, this==" << this << ", ec=" << ec << ", msg=" << mWriteMsgs.front().c_str() << std::endl;
#endif
           mWriteMsgs.pop_front();
#ifdef DEBUG_PRINT
           std::cout << "Session::DoWrite 3, this==" << this << ", ec=" << ec << std::endl;
#endif
           if (!ec)
           {
#ifdef DEBUG_PRINT
               std::cout << "Session::DoWrite 3, this==" << this << ", ec=" << ec << std::endl;
#endif
               //if (!mWriteMsgs.empty())
               //    DoWrite();
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
