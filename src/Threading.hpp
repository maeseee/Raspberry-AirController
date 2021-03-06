#pragma once

#include <atomic>
#include <thread>

namespace threading
{

class Threading
{
public:
    Threading(const size_t intervall);
    virtual ~Threading();

    virtual void stopThreading() final;
    virtual void waitStopped() final;

    virtual void recall() = 0;

    /**
     * @brief setInitialized call as last call in the constructor in a derived class
     */
    void setInitialized();

private:
    void threadFn();

    const size_t m_intervall;
    std::thread m_thread;
    size_t m_timeCounter{0};

    std::atomic_bool m_isInitialized{false};
    std::atomic_bool m_stopThread{false};
};
} // namespace threading
