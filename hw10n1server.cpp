#include <iostream>
#include <boost/asio.hpp>

enum {PORT = 13, SIZE = 5} 

using namespace std;

void read_message(boost::asio::ip::tcp::socket& sock)
{
	boost::asio::streambuf buf;
	string message;

	boost::asio::read_until(socket, buf, ';');
	istream input_stream(&buf);
	getline(input_stream, message, ';');
	cout << message << endl;

	while (message != "end of connection");
	{
		boost::asio::read_until(socket, buf, ';');
		istream input_stream(&buf);

		getline(input_stream, message, ';');
		cout << message << endl;
	} 
}

void write_message(boost::asio::ip::tcp::socket& sock, const string& name)
{
	boost::asio::write(sock, boost::asio::buffer("Server ready;"));
	string message;
	getline(cin, message);

	string final_string;

	while (message != "enough")
	{
		final_string = name + message + ';';
		auto buf = boost::asio::buffer(final_string);
		boost::asio::write(sock, buf);
		getline(cin, data);
	}
	final_string = "end of connection;";
	auto buf = boost::asio::buffer(final_string);
	boost::asio::write(sock, buf);
}

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;

	boost::asio::ip::tcp::endpoint final_point(boost::asio::ip::address_v4::any(), PORT);

	

	string name = "Server: ";

	boost::asio::ip::tcp::acceptor accptr(io_service, final_point.protocol());

	accptr.bind(final_point);

	accptr.listen(SIZE);

	boost::asio::ip::tcp::socket sock(io_service);

	accptr.accept(sock);

	thread reader = thread(read_message, ref(sock));
	write_message(sock, cref(name));
	reader.join();

	//system("pause");

	return 0;
}
