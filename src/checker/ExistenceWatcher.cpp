#include "ExistenceWatcher.h"
#include <QFileInfo>

CheckResult ExistenceWatcher::check(const QString& filePath) {
    QFileInfo info(filePath);
    info.refresh();
    const bool exists = info.exists();
    
    if (m_firstCheck.contains(filePath)) {
        return CheckResult(CheckEvent::NONE, filePath);
    }

    m_firstCheck[filePath] = true;

    if (exists) {
        return CheckResult(CheckEvent::FILE_EXISTS, filePath, info.size());
    }
    return CheckResult(CheckEvent::FILE_DELETED, filePath);
}

void ExistenceWatcher::reset(const QString& filePath) {
    m_firstCheck.remove(filePath);
}