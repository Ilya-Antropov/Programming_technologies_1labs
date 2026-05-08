#include "SizeWatcher.h"
#include <QFileInfo>

CheckResult SizeWatcher::check(const QString& filePath) {
    QFileInfo info(filePath);
    info.refresh();

    if (!info.exists()) {
        return CheckResult(CheckEvent::NONE, filePath);
    }

    const qint64 currentSize = info.size();
    const QDateTime currentMod = info.lastModified();

    if (!m_lastSize.contains(filePath)) {
        m_lastSize[filePath] = currentSize;
        m_lastModified[filePath] = currentMod;
        return CheckResult(CheckEvent::NONE, filePath);
    }

    const qint64 prevSize = m_lastSize.value(filePath);
    const QDateTime prevMod = m_lastModified.value(filePath);

    if (currentSize != prevSize) {
        m_lastSize[filePath] = currentSize;
        m_lastModified[filePath] = currentMod;
        return CheckResult(CheckEvent::FILE_CHANGED, filePath,
                           currentSize, prevSize);
    }

    if (currentMod != prevMod) {
        m_lastModified[filePath] = currentMod;
        return CheckResult(CheckEvent::FILE_TOUCHED, filePath,
                           currentSize, currentSize);
    }

    return CheckResult(CheckEvent::NONE, filePath);
}

void SizeWatcher::reset(const QString& filePath) {
    m_lastSize.remove(filePath);
    m_lastModified.remove(filePath);
}