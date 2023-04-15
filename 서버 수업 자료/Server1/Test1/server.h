#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "Message.h"
#include <string>


using boost::asio::ip::tcp;


typedef std::deque<Message> Message_queue;

class User
{
public:
	virtual ~User() {}
	virtual void deliver(const Message& msg) = 0;
};

typedef std::shared_ptr<User> User_ptr;


class Room
{
public:
	void join(User_ptr user)
	{
		_list_User.insert(user);
		for (auto msg : _recent_msgs)
		{
			user->deliver(msg);
		}
	 }
	void leave(User_ptr user)
	{
		_list_User.erase(user);
	}

	void deliver(const Message& msg)
	{
		_recent_msgs.push_back(msg);

		while (_recent_msgs.size() > max_recent_msgs)
			_recent_msgs.pop_front();

		for (auto user : _list_User)
			user->deliver(msg);
	}
	

private:
	std::set<User_ptr> _list_User;
	enum { max_recent_msgs = 100 };
	Message_queue _recent_msgs;

};



class Session : public User,
	public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket,Room& room)
		: _socket(std::move(socket)), _room(room)
	{

	}

	void start()
	{
		_room.join(shared_from_this());
		do_read_header();
	}

	void deliver( const Message& msg)
	{
		bool write = !write_msgs.empty();
		write_msgs.push_back(msg);
		if (!write)
		{
			do_write();
		}
	}
private:
	tcp::socket _socket;
	Room& _room;
	Message _read_msg;
	Message_queue write_msgs;
	

	void do_read_header()
	{
		auto self(shared_from_this());
		// 스마트 포인트 속박용
		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.data(), Message::header_length),
			[this, self](boost::system::error_code e, std::size_t)
			{
				if (!e && _read_msg.decode_header())
				{
					do_read_body();
				}
				else
				{
					_room.leave(shared_from_this());
				}

			});
		

	}
	void do_read_body()
	{

		auto self(shared_from_this());
		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
			[this, self](boost::system::error_code e, std::size_t)
			{

				if (!e)
				{
					char temp[50] = "";
					char str[50] = "player";

					for (int i = 0;i < _read_msg.body_length();i++)
					{
						if (_read_msg.body()[i] != NULL)
							temp[i] = _read_msg.body()[i];
						else
						{
							break;
						}
					}
					int num = 0;
					for (int i = 6 ;i < 7 + _read_msg.body_length() ; i++)
					{
						if (i == 6)
							str[i] = ':';
						else
						{
							str[i] = temp[num];
							num++;
						}

					}
					_read_msg.setbody(str, 7 + _read_msg.body_length());

					printf_s(str);
					printf_s("\n");
					_read_msg.encode_header();
					_room.deliver(_read_msg);
					do_read_header();
				
				}
				else
				{
					_room.leave(shared_from_this());
				}
				

			});
	}
	void do_write()
	{
		auto self(shared_from_this());
		boost::asio::async_write(_socket,
			boost::asio::buffer(write_msgs.front().data(),
				write_msgs.front().length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					write_msgs.pop_front();
					if (!write_msgs.empty())
					{
						do_write();
					}
				}
				else
				{
					_room.leave(shared_from_this());
				}
			});
	}
};

class Server
{

public:
	Server(boost::asio::io_context& context,
		const tcp::endpoint& endpoint)
		:acceptor(context,endpoint)

	{
		GetData();
	}



	


private:
	tcp::acceptor acceptor;
	Room _room;



	void GetData()
	{
		acceptor.async_accept(
			[this](boost::system::error_code e, tcp::socket socket)
			{
				if (!e)
				{
					// session활성화
					std::make_shared<Session>(std::move(socket), _room)->start();
					
				}
				GetData();

			});
	}


};





