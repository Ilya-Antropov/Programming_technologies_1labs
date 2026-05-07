#include "PollWorker.h"

PollWorker::PollWorker(std::function<void()> callback, int intervalMs)
    : m_callback(std::move(callback))
      , m_intervalMs(intervalMs)
      , m_running(false) {
}

void PollWorker::start() {
    m_running.store(true);
    m_thread = std::thread(&PollWorker::run, this);
}

bool PollWorker::isRunning() const
{
    return m_running.load();
}
