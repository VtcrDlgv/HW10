#include <iostream>
#include <boost/asio.hpp>

enum {PORT = 13} 

using namespace std;

void read_message(boost::asio::ip::tcp::socket& sock)
{
	boost::asio::streambuf buffer;
	string message;

	boost::asio::read_until(sock, buffer, ';');
	istream input_stream(&buffer);

	getline(input_stream, message, ';');
	cout << message << endl;

	while (message != "end of connection")
	{
		boost::asio::read_until(sock, buffer, ';');
		istream input_stream(&buffer);

		getline(input_stream, message, ';');
		cout << message << endl;
	}
}

void write_message(boost::asio::ip::tcp::socket& sock, const string& name)
{
	string message;
	getline(cin, message);

	string final_string;
	while (message != "enough")
	{
		final_string = name + message + ';';
		auto buf = boost::asio::buffer(final_string);
		boost::asio::write(sock, buf);
		getline(cin, message);
	}
	final_string = "end of connection;";
	auto buf = boost::asio::buffer(final_string);
	boost::asio::write(sock, buf);
}

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;

	string ip_address = "127.0.0.1";

	string name;

	cout << "Please, input name:" << endl;

	getline(cin, name);

	boost::asio::ip::tcp::endpoint final_point(boost::asio::ip::address::from_string(ip_address), PORT);

	boost::asio::ip::tcp::socket sock(io_service, final_point.protocol());

	sock.connect(final_point);

	thread reader = thread(read_message, ref(sock));
	write_message(sock, cref(name));
	reader.join();

	return 0;
}
