//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
//#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>
#include "connection.h"
#include "request_handler.h"
#include "chat_message.h"
#include <string>
//using namespace http;
//using namespace server3;

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        for (auto msg : recent_msgs_)
            participant->deliver(msg);
    }

    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }

    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant : participants_)
            participant->deliver(msg);
    }

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session
    : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room)
        : socket_(std::move(socket)),
        room_(room)
    {
    }

    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const chat_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.data(), chat_message::header_length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && read_msg_.decode_header())
                {
                    do_read_body();
                }
                else
                {
                    room_.leave(shared_from_this());
                }
            });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    char temp[50] = " ";
                    char str[50] = "player";


                    for (int i = 0; i < read_msg_.body_length(); i++)
                    {
                        if (read_msg_.body()[i] != NULL)
                            temp[i] = read_msg_.body()[i];
                        else
                        {
                            break;
                        }
                    }
                    int num = 0;
                    for (int i = 6; i < 7 + read_msg_.body_length(); i++)
                    {
                        if (i == 6)
                            str[i] = ':';
                        else
                        {
                            str[i] = temp[num];
                            num++;

                        }
                    }

                    read_msg_.setBody(str, 7 + read_msg_.body_length());

                    printf_s(str);
                    printf_s("\n");
                    read_msg_.encode_header();
                    room_.deliver(read_msg_);
                    do_read_header();
                }
                else
                {
                    room_.leave(shared_from_this());
                }
            });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
                write_msgs_.front().length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty())
                    {
                        do_write();
                    }
                }
                else
                {
                    room_.leave(shared_from_this());
                }
            });
    }

    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------














namespace http {
    namespace server3 {

        class server
            : private boost::noncopyable
        {
        public:
            /// Construct the server to listen on the specified TCP address and port, and
            /// serve up files from the given directory.
            explicit server(const std::string& address, const std::string& port,
                const std::string& doc_root, std::size_t thread_pool_size);

            /// Run the server's io_context loop.
            void run();

        private:
            /// Initiate an asynchronous accept operation.
            void start_accept();

            /// Handle completion of an asynchronous accept operation.
            void handle_accept(const boost::system::error_code& e);

            /// Handle a request to stop the server.
            void handle_stop();

            /// The number of threads that will call io_context::run().
            std::size_t thread_pool_size_;

            /// The io_context used to perform asynchronous operations.
            boost::asio::io_context io_context_;

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections.
            boost::asio::ip::tcp::acceptor acceptor_;

            /// The next connection to be accepted.
            connection_ptr new_connection_;

            /// The handler for all incoming requests.
            request_handler request_handler_;
        };
    }
}

//----------------------------------------------------------------------

