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
    DoProcessCommand();
}

void Session::Stop()
{
    mSocket.close();
    mContext.Stop();
}

void Session::DoProcessCommand()
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
            DoWrite();    

            if (!ec)
                DoProcessCommand();
            else
                Stop();
        });
}

void Session::GetWriteQueue()
{
    auto statuses = mContext.GetOutboundQueue();
    while (statuses.empty())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        statuses = mContext.GetOutboundQueue();
    }
    std::stringstream str;
    for (const auto& status : statuses)
        str << status;
#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue, this==" << this << ", str=" << str.str() << std::endl;
#endif
    mWriteMsgs.push_back(str.str());
}
  
void Session::DoWrite()
{
    if (mWriteMsgs.empty())
        return;

    boost::asio::async_write(mSocket,
        boost::asio::buffer(mWriteMsgs.front().c_str(),
            mWriteMsgs.front().size()),
    [this](boost::system::error_code ec, std::size_t /*length*/)
       {
#ifdef DEBUG_PRINT
           std::cout << "Session::DoWrite, this==" << this << ", ec=" << ec << std::endl;
#endif
           mWriteMsgs.pop_front();

           if (!ec)
           {
#ifdef DEBUG_PRINT
               std::cout << "Session::DoWrite 2, this==" << this << ", ec=" << ec << std::endl;
#endif
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
