#pragma once

#include <QString>
#include <mutex>

enum class LogLevel {
    DEBUG_LVL,
    INFO,
    WARNING,
    ERROR_LVL
};

class AppLogger {
public:
    AppLogger(const AppLogger&) = delete;

    AppLogger& operator=(const AppLogger&) = delete;

    AppLogger(AppLogger&&) = delete;

    AppLogger& operator=(AppLogger&&) = delete;

    static AppLogger& instance();

    void log(LogLevel level, const QString& message);

private:
    AppLogger();

    ~AppLogger() = default;

    LogLevel m_minLevel;
    std::mutex m_mutex;
};