#include "PollWorker.h"
#include <chrono>

PollWorker::PollWorker(std::function<void()> callback, int intervalMs)
    : m_callback(std::move(callback))
      , m_intervalMs(intervalMs)
      , m_running(false) {
}

void PollWorker::start() {
    m_running.store(true);
    m_thread = std::thread(&PollWorker::run, this);
}

bool PollWorker::isRunning() const {
    return m_running.load();
}

void PollWorker::run() {
    while (m_running.load()) {
        if (m_callback) {
            m_callback();
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(m_intervalMs));
    }
}

void PollWorker::stop() {
    if (!m_running.load()) { return; }

    m_running.store(false);

    if (m_thread.joinable()) {
        m_thread.join();
    }
}

PollWorker::~PollWorker() {
    stop();
}