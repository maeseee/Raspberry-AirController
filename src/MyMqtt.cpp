#include "MyMqtt.hpp"

#include <cstring>
#include <iostream>

myMosq::myMosq(const char* id, const char* topic, const char* host, const int port)
    : mosquittopp(id)
{
    mosqpp::lib_init(); // Mandatory initialization for mosquitto library
    m_keepalive = 60;   // Basic configuration setup for myMosq class
    m_id = id;
    m_port = port;
    m_host = host;
    m_topic = topic;
    connect_async(host, // non blocking connection to broker request
                  port, m_keepalive);
    loop_start(); // Start thread managing connection / publish / subscribe
}

myMosq::~myMosq()
{
    loop_stop();           // Kill the thread
    mosqpp::lib_cleanup(); // Mosquitto library cleanup
}

bool myMosq::send_message(const char* _message)
{
    // Send message - depending on QoS, mosquitto lib managed re-submission this the thread
    //
    // * NULL : Message Id (int *) this allow to latter get status of each message
    // * topic : topic to be used
    // * lenght of the message
    // * message
    // * qos (0,1,2)
    // * retain (boolean) - indicates if message is retained on broker or not
    // Should return MOSQ_ERR_SUCCESS
    int ret = publish(nullptr, m_topic, std::strlen(_message), _message, 1, false);
    return (ret == MOSQ_ERR_SUCCESS);
}

void myMosq::on_disconnect(int rc)
{
    std::cout << ">> myMosq - disconnection(" << rc << ")" << std::endl;
}

void myMosq::on_connect(int rc)
{
    if (rc == 0) {
        std::cout << ">> myMosq - connected with server" << std::endl;
    } else {
        std::cout << ">> myMosq - Impossible to connect with server(" << rc << ")" << std::endl;
    }
}

void myMosq::on_publish(int mid)
{
    std::cout << ">> myMosq - Message (" << mid << ") succeed to be published " << std::endl;
}
