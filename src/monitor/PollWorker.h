#pragma once

#include <thread>
#include <atomic>
#include <functional>

class PollWorker {
public:
    explicit PollWorker(std::function<void()> callback,
                        int intervalMs = 100);

    PollWorker(const PollWorker&) = delete;

    PollWorker& operator=(const PollWorker&) = delete;

    void start();

    bool isRunning() const;

private:
    std::function<void()> m_callback;
    int m_intervalMs;
    std::thread m_thread;
    std::atomic<bool> m_running;
};