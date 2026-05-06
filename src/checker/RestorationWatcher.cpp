#include "RestorationWatcher.h"
#include <QFileInfo>

CheckResult RestorationWatcher::check(const QString& filePath) {
    QFileInfo info(filePath);
    info.refresh();
    const bool exists = info.exists();

    if (!m_firstCheck.contains(filePath)) {
        m_firstCheck[filePath] = true;
        m_wasPresent[filePath] = exists;
        return CheckResult(CheckEvent::NONE, filePath);
    }

    const bool wasPrev = m_wasPresent.value(filePath, false);
    if (wasPrev == exists) {
        return CheckResult(CheckEvent::NONE, filePath);
    }

    m_wasPresent[filePath] = exists;

    if (!exists) {
        return CheckResult(CheckEvent::FILE_DELETED, filePath);
    }

    return CheckResult(CheckEvent::FILE_RESTORED, filePath, info.size());
}

void RestorationWatcher::reset(const QString& filePath) {
    m_wasPresent.remove(filePath);
    m_firstCheck.remove(filePath);
}
