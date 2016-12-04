#include "HttpServer.hpp"

#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

Session::Session(boost::asio::ip::tcp::socket socket)
    : m_socket(std::move(socket)) {}

void Session::start() { doRead(); }

void Session::doRead() {
  auto self(shared_from_this());
  m_socket.async_read_some(
      boost::asio::buffer(m_data, MAX_LENGTH),
      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          doWrite(length);
        }
      });
}

void Session::doWrite(std::size_t length) {
  auto self(shared_from_this());
  boost::asio::async_write(
      m_socket, boost::asio::buffer(m_data, length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          doRead();
        }
      });
}

Server::Server(boost::asio::io_service &io_service, short port)
    : m_acceptor(io_service, boost::asio::ip::tcp::endpoint(
                                 boost::asio::ip::tcp::v4(), port)),
      m_socket(io_service) {
  doAccept();
}

void Server::doAccept() {

  m_acceptor.async_accept(m_socket, [this](boost::system::error_code ec) {
    if (!ec) {
      std::make_shared<Session>(std::move(m_socket))->start();
    }

    doAccept();
  });
}

int initHttpServer() {
  try {
    boost::asio::io_service io_service;

    Server s(io_service, 5000);

    io_service.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
