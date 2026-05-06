#include "ExistenceWatcher.h"
#include <QFileInfo>

CheckResult ExistenceWatcher::check(const QString& filePath) {
    QFileInfo info(filePath);
    info.refresh();
    const bool exists = info.exists();

    const bool isFirst = !m_firstCheck.contains(filePath);
    if (isFirst) {
        m_firstCheck[filePath] = true;
        m_lastExists[filePath] = exists;

        if (exists) {
            return CheckResult(CheckEvent::FILE_EXISTS, filePath, info.size());
        }
        return CheckResult(CheckEvent::FILE_DELETED, filePath);
    }

    const bool wasExists = m_lastExists.value(filePath, false);
    if (wasExists == exists) {
        return CheckResult(CheckEvent::NONE, filePath);
    }

    m_lastExists[filePath] = exists;

    if (!exists) {
        return CheckResult(CheckEvent::FILE_DELETED, filePath);
    }
    return CheckResult(CheckEvent::NONE, filePath);
}

void ExistenceWatcher::reset(const QString& filePath) {
    m_lastExists.remove(filePath);
    m_firstCheck.remove(filePath);
}