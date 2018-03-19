#pragma once

#include <Mosquitto/cpp/mosquittopp.h>
// #include <Mosquitto/mosquitto.h>

class myMosq : public mosqpp::mosquittopp
{
private:
    const char* m_host;
    const char* m_id;
    const char* m_topic;
    int m_port;
    int m_keepalive;

    void on_connect(int rc) override;
    void on_disconnect(int rc) override;
    void on_publish(int mid) override;

public:
    myMosq(const char* id, const char* topic, const char* host, const int port);
    ~myMosq();
    bool send_message(const char* _message);
};
