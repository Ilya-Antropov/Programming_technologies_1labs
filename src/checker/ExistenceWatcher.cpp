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

    // еще добавиться сравнение следующих опросов с предыдущим
}

// тут будет еще reset