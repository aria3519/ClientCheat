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
		_list_User.push_back(user);
		for (auto msg : _recent_msgs)
		{
			user->deliver(msg);
		}
	 }
	void leave(User_ptr user)
	{
		_list_User.remove(user);	
	}

	void deliver(const Message& msg)
	{
		_recent_msgs.push_back(msg);

		while (_recent_msgs.size() > max_msgs)
			_recent_msgs.pop_front();
	}
	

private:
	std::list<User_ptr> _list_User;
	const int max_msgs = 100;
	Message_queue _recent_msgs;

};

class Player
{
public:
	

private:

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
						//if(_read_msg.body()[i]!=)
					}
				}

			});
	}
	void do_write()
	{

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



	void GetData()
	{
		acceptor.async_accept(
			[this](boost::system::error_code e, tcp::socket socket)
			{
				if (!e)
				{
					// session활성화 
					
				}
				GetData();

			});
	}


};





