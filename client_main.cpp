#include <boost/asio.hpp>

#include "client.h"

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

        client.DoJob();
        client.Close();

        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
