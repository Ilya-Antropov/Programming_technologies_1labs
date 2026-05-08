#pragma once

#include <QString>

class AppConfig {
public:
    AppConfig(const AppConfig&) = delete;

    AppConfig& operator=(const AppConfig&) = delete;

    AppConfig(AppConfig&&) = delete;

    AppConfig& operator=(AppConfig&&) = delete;

    static AppConfig& instance();

    void load(const QString& iniPath);

    int pollIntervalMs() const;

    QString logLevel() const;

private:
    AppConfig();

    ~AppConfig() = default;

    int m_pollIntervalMs;
    QString m_logLevel;
};