#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <deque>
#include <set>
#include "message.h"


using boost::asio::ip::tcp;


class Players
{
public:
	virtual ~Players() {}
	virtual void deliver(const message& msg) = 0;
};
typedef std::shared_ptr<Players>Players_ptr;
typedef std::deque<message> message_qu;




class Sesstion_ : public std::enable_shared_from_this<Sesstion_>
{
public:
	Sesstion_(tcp::socket socket, Room_& room)
		: socket(std::move(socket)),room(room)
	{

	}
	~Sesstion_() 
	{

	}


	void start()
	{
		
		room.join(shared_from_this());
		read_head();
		
	}
private:
	tcp::socket socket;
	Room_& room;

	void read_head()
	{

	}
};


class  Server_
{
public:
	Server_(boost::asio::io_context& comtext,
		tcp::endpoint& endpoint)
		: acceptor_(comtext, endpoint)
	{
		Accept();
	}
	~Server_()
	{
		
	}

private:
	tcp::acceptor acceptor_;
	Room_ room_;

	void Accept()
	{
		acceptor_.async_accept(
			[this](boost::system::error_code err, tcp::socket socket)
			{
				if (!err)
				{
					std::make_shared<Sesstion_>(std::move(socket), room_)->start();
				}

		Accept();
			});
	}
};



 


 class  Room_
 {
 public:
	 Room_()
	 {

	 }
	 ~Room_()
	 {

	 }

	 void join(Players_ptr _pls)
	 {
		 players.insert(_pls);
		 for (auto msg : recent_msgs_)
		 {
			 _pls->deliver(msg);
		 }
		
	 }


	 void deliver()
	 {

	 }

 private:
	 std::set<Players_ptr> players;
	 message_qu recent_msgs_;
 };



  