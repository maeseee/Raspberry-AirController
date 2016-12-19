#include <SysLogger.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http_server {

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(boost::asio::ip::tcp::socket socket,
          const logger::SysLoggerPtr &sysLogger);

  void start();

private:
  void doRead();

  void doWrite(std::size_t length);

  static const size_t MAX_LENGTH = 1024;

  boost::asio::ip::tcp::socket m_socket;
  char m_data[MAX_LENGTH];

  logger::SysLoggerPtr m_logger;
};

class Server {
public:
  Server(boost::asio::io_service &io_service, short port,
         const logger::SysLoggerPtr &sysLogger);

private:
  void doAccept();

  logger::SysLoggerPtr m_logger;

  boost::asio::ip::tcp::acceptor m_acceptor;
  boost::asio::ip::tcp::socket m_socket;
};

int initHttpServer(const logger::SysLoggerPtr &sysLogger);
}
