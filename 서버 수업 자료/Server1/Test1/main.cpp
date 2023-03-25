#include "Server.h"

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_context context;
		//std::list<Server> server;

		tcp::endpoint end(tcp::v4(), 5555);
		// 서버가 context 랑 endpoint 받아야함
		Server server(context,end);

		context.run();


	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}



	return 0;
}