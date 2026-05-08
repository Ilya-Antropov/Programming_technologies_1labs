#pragma once

#include <QString>
#include <QMap>
#include <QtCore/QDateTime>
#include "IntFileChecker.h"


class SizeWatcher : public IntFileChecker {
public:
    SizeWatcher() = default;

    ~SizeWatcher() override = default;

    CheckResult check(const QString& filePath) override;

    void reset(const QString& filePath) override;

private:
    QMap<QString, qint64> m_lastSize;
    QMap<QString, QDateTime> m_lastModified;
};