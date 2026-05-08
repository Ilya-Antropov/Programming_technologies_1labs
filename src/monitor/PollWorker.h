#pragma once

#include <thread>
#include <atomic>
#include <functional>

class PollWorker {
public:
    explicit PollWorker(std::function<void()> callback,
                        int intervalMs = 100);

    ~PollWorker();

    PollWorker(const PollWorker&) = delete;

    PollWorker& operator=(const PollWorker&) = delete;

    void start();

    void stop();

    bool isRunning() const;

private:
    void run();

    std::function<void()> m_callback;
    int m_intervalMs;
    std::thread m_thread;
    std::atomic<bool> m_running;
};