#include "Threading.hpp"

#include <cassert>
#include <unistd.h> // for sleep

namespace threading
{

Threading::Threading(const size_t intervall)
    : m_intervall{intervall}
    , m_thread(std::thread(&Threading::threadFn, this))
{
}

Threading::~Threading()
{
    stopThreading();
    waitStopped();
}

void Threading::stopThreading()
{
    m_stopThread = true;
}

void Threading::waitStopped()
{
    assert(true == m_stopThread);
    m_thread.join();
}

void Threading::setInitialized()
{
    m_isInitialized = true;
}

void Threading::threadFn()
{
    // wait until the derived object is completely constructed
    while (!m_isInitialized) {
        sleep(1);
    }

    while (!m_stopThread) {
        if (0 == m_timeCounter) {
            recall();
        }
        ++m_timeCounter;
        m_timeCounter %= m_intervall;
        sleep(1);
    }
}
}
