//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server.h"

int main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        /*if (argc != 5)
        {
            std::cerr << "Usage: http_server <address> <port> <threads> <doc_root>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 1 .\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 1 .\n";
            return 1;
        }*/

        // Initialise the server.
        std::size_t num_threads = 1;
        // 127.0.0.1 현재 컴퓨터의 local ip  로 넣어줌 
        http::server3::server s("192.168.0.223", "5555", "C:\\권민재\\ClientCheat\\서버 수업 자료", num_threads);
        
        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}