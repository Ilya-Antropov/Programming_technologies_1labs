#include "AppLogger.h"

AppLogger& AppLogger::instance() {
    static AppLogger logger;
    return logger;
}

AppLogger::AppLogger()
    : m_minLevel(LogLevel::INFO) {
}

void AppLogger::log(LogLevel level, const QString& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (level < m_minLevel) {
        return;
    }
}