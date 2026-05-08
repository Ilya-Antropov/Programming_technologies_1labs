#pragma once

#include <QString>
#include "IntFileChecker.h"
#include <QMap>

class ExistenceWatcher : public IntFileChecker {
public:
    ExistenceWatcher() = default;

    ~ExistenceWatcher() override = default;

    CheckResult check(const QString& filePath) override;

    void reset(const QString& filePath) override;

private:
    QMap<QString, bool> m_firstCheck;
};