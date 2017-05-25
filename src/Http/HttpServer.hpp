#include <SysLogger.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

// FWD
namespace time_trigger
{
class OneTimeTrigger;
using OneTimeTriggerPtr = std::shared_ptr<OneTimeTrigger>;
}

// Class

namespace http_server
{

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket,
            const time_trigger::OneTimeTriggerPtr& oneTimeTrigger,
            const logger::SysLoggerPtr& sysLogger);

    void start();

private:
    void doRead();

    /**
     * @brief processData interpret the received data
     * @param receivedData
     * @return answer
     */
    std::string processData(const std::string& receivedData) const;

    void doWrite();

    static const size_t MAX_LENGTH = 1024;

    boost::asio::ip::tcp::socket m_socket;
    char m_rxData[MAX_LENGTH];
    char m_txData[MAX_LENGTH];

    const time_trigger::OneTimeTriggerPtr m_oneTimeTrigger;
    const logger::SysLoggerPtr m_logger;
};

class Server
{
public:
    Server(boost::asio::io_service& io_service,
           short port,
           const time_trigger::OneTimeTriggerPtr& oneTimeTrigger,
           const logger::SysLoggerPtr& sysLogger);

private:
    void doAccept();

    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;

    const time_trigger::OneTimeTriggerPtr m_oneTimeTrigger;
    const logger::SysLoggerPtr m_logger;
};

int initHttpServer(const time_trigger::OneTimeTriggerPtr& oneTimeTrigger, const logger::SysLoggerPtr& sysLogger);
}
