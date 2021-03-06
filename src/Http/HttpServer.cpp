#include "HttpServer.hpp"
#include <Controller/OneTimeTrigger.hpp>
#include <Utility/Constants.hpp>

#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

namespace http_server
{

Session::Session(boost::asio::ip::tcp::socket socket,
                 const time_trigger::OneTimeTriggerPtr& oneTimeTrigger,
                 const logger::SysLoggerPtr& sysLogger)
    : m_socket(std::move(socket))
    , m_oneTimeTrigger(oneTimeTrigger)
    , m_logger(sysLogger)
{
}

void Session::start()
{
    doRead();
}

void Session::doRead()
{
    std::fill(m_rxData, m_rxData + MAX_LENGTH, 0);
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_rxData, MAX_LENGTH),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     std::fill(m_txData, m_txData + MAX_LENGTH, 0);
                                     std::cout << "HttpServer received: " << m_rxData << std::endl;
                                     std::string answer = processData(std::string(m_rxData));
                                     strcpy(m_txData, answer.c_str());
                                     doWrite();
                                 }
                             });
}

std::string Session::processData(const std::string& receivedData) const
{
    std::stringstream ss;
    auto ids = m_logger->getIds(receivedData);
    if (0 < ids.size()) {
        std::string space = "";
        for (const size_t& id : ids) {
            std::string value = m_logger->getValueFromId(id);
            if ("" != value) {
                std::string valueName = m_logger->getNameFromId(id);
                ss << space;
                ss << valueName << ": " << value;
                space = ", ";
            }
        }
        if ("" == space) {
            ss << "No data received so far";
        }
    } else if ("ListAll" == receivedData) {
        auto allIds = m_logger->getAllIds();
        std::string space = "";
        for (const size_t& id : allIds) {
            std::string value = m_logger->getNameFromId(id);
            ss << space;
            ss << value;
            space = ", ";
        }
    } else if ("MainSystemOn" == receivedData) {
        m_oneTimeTrigger->addTrigger(ONE_TIME_ON_DURATION);
        ss << "System has been switched on";
    } else {
        ss << "No command to execute " << receivedData;
    }

    std::cout << ss.str() << std::endl;
    return ss.str();
}

void Session::doWrite()
{
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(m_txData, strlen(m_txData)),
                             [self](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     // doRead();
                                 }
                             });
}

Server::Server(boost::asio::io_service& io_service,
               short port,
               const time_trigger::OneTimeTriggerPtr& oneTimeTrigger,
               const logger::SysLoggerPtr& sysLogger)
    : m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    , m_socket(io_service)
    , m_oneTimeTrigger(oneTimeTrigger)
    , m_logger(sysLogger)
{
    doAccept();
}

void Server::doAccept()
{

    m_acceptor.async_accept(m_socket, [this](boost::system::error_code ec) {
        if (!ec) {
            std::make_shared<Session>(std::move(m_socket), m_oneTimeTrigger, m_logger)->start();
        }

        doAccept();
    });
}

size_t initHttpServer(const time_trigger::OneTimeTriggerPtr& oneTimeTrigger, const logger::SysLoggerPtr& sysLogger)
{
    try {
        boost::asio::io_service io_service;

        Server s(io_service, 5000, oneTimeTrigger, sysLogger);

        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
} // namespace http_server
