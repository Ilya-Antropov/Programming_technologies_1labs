#include "SizeWatcher.h"
#include <QFileInfo>

CheckResult SizeWatcher::check(const QString& filePath) {
    QFileInfo info(filePath);
    info.refresh();

    if (!info.exists()) {
        return CheckResult(CheckEvent::NONE, filePath);
    }

    const qint64 currentSize = info.size();

    if (!m_lastSize.contains(filePath)) {
        m_lastSize[filePath] = currentSize;
        return CheckResult(CheckEvent::NONE, filePath);
    }

    const qint64 prevSize = m_lastSize.value(filePath);

    if (currentSize == prevSize) {
        return CheckResult(CheckEvent::NONE, filePath);
    }

    m_lastSize[filePath] = currentSize;

    return CheckResult(CheckEvent::FILE_CHANGED, filePath, currentSize, prevSize);
}

void SizeWatcher::reset(const QString& filePath) {
    m_lastSize.remove(filePath);
}
