#include "AppLogger.h"
#include <QDateTime>
#include <QTextStream>

AppLogger& AppLogger::instance() {
    static AppLogger logger;
    return logger;
}

AppLogger::AppLogger()
    : m_minLevel(LogLevel::INFO) {
}

void AppLogger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_minLevel = level;
}

void AppLogger::log(LogLevel level, const QString& message) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (level < m_minLevel) {
        return;
    }

    const QString ts = QDateTime::currentDateTime()
        .toString(QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz"));

    QTextStream out(stdout);
    out << QStringLiteral("[%1] [%2] %3\n")
        .arg(ts)
        .arg(levelToTag(level))
        .arg(message);
    out.flush();
}

void AppLogger::debug(const QString& m) { log(LogLevel::DEBUG_LVL, m); }
void AppLogger::info(const QString& m) { log(LogLevel::INFO, m); }
void AppLogger::warning(const QString& m) { log(LogLevel::WARNING, m); }
void AppLogger::error(const QString& m) { log(LogLevel::ERROR_LVL, m); }

QString AppLogger::levelToTag(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG_LVL:
            return QStringLiteral("DEBUG  ");
        case LogLevel::INFO:
            return QStringLiteral("INFO   ");
        case LogLevel::WARNING:
            return QStringLiteral("WARNING");
        case LogLevel::ERROR_LVL:
            return QStringLiteral("ERROR  ");
    }
    return QStringLiteral("UNKNOWN");
}