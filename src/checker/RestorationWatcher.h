#pragma once

#include <QMap>
#include <QString>
#include "IntFileChecker.h"

class RestorationWatcher : public IntFileChecker {
public:
    RestorationWatcher() = default;

    ~RestorationWatcher() override = default;

    CheckResult check(const QString& filePath) override;

    void reset(const QString& filePath) override;

private:
    QMap<QString, bool> m_wasPresent;
    QMap<QString, bool> m_firstCheck;
};