#include "chat_server.h"


int main(int argc, char* argv[])
{
    try
    {
       /* if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }*/

        boost::asio::io_context io_context;

        std::list<chat_server> servers;
        // 포트를 여러개 
        /*for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), 5555);
            servers.emplace_back(io_context, endpoint);
        }*/
        tcp::endpoint endpoint(tcp::v4(), 5555);
        chat_server serv(io_context, endpoint);
       
        

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}