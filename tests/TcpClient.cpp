#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

using boost::asio::ip::tcp;

static const size_t MAX_LENGTH = 1024;

int main(int argc, char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::socket s(io_service);
    tcp::resolver resolver(io_service);
    boost::asio::connect(s, resolver.resolve({argv[1], argv[2]}));

    std::cout << "Enter message: ";
    char request[MAX_LENGTH];
    std::cin.getline(request, MAX_LENGTH);
    size_t requestLength = std::strlen(request);
    boost::asio::write(s, boost::asio::buffer(request, requestLength));

    boost::asio::streambuf response;
    boost::asio::read_until(s, response, "\0");

    std::cout << "Reply is: ";
    std::istream is(&response);
    std::string line;
    std::getline(is, line);

    std::cout << line << std::endl;
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
