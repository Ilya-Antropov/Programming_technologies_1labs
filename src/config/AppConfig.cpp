#include "AppConfig.h"
#include <QSettings>
#include <QFile>
#include "../logger/AppLogger.h"

AppConfig& AppConfig::instance() {
    static AppConfig config;
    return config;
}

AppConfig::AppConfig()
    : m_pollIntervalMs(100)
      , m_logLevel(QStringLiteral("INFO")) {
}


void AppConfig::load(const QString& iniPath) {
    if (!QFile::exists(iniPath)) {
        AppLogger::instance().warning(
            QStringLiteral("AppConfig: '%1' не найден, defaults").arg(iniPath));
        return;
    }

    QSettings settings(iniPath, QSettings::IniFormat);

    m_pollIntervalMs = settings.value(
        QStringLiteral("General/poll_interval_ms"), 100).toInt();
    m_logLevel = settings.value(
        QStringLiteral("General/log_level"),
        QStringLiteral("INFO")).toString();
}

int AppConfig::pollIntervalMs() const { return m_pollIntervalMs; }
QString AppConfig::logLevel() const { return m_logLevel; }