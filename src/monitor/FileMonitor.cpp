#include <QString>
#include <QFileInfo>
#include <algorithm>
#include "FileMonitor.h"
#include "../logger/AppLogger.h"

FileMonitor::FileMonitor(QObject* parent)
    : QObject(parent)
      , m_notifier(nullptr) {
}

void FileMonitor::addChecker(std::unique_ptr<IntFileChecker> checker) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_checkers.push_back(std::move(checker));
}

void FileMonitor::setNotifier(IntNotifier* notifier) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_notifier = notifier;
}

bool FileMonitor::addFile(const QString& filePath) {
    if (filePath.trimmed().isEmpty()) {
        AppLogger::instance().warning(
            QStringLiteral("FileMonitor::addFile: пустой путь"));
        return false;
    }

    QFileInfo info(filePath);
    if (info.exists() && !info.isFile()) {
        AppLogger::instance().warning(
            QStringLiteral("FileMonitor::addFile: '%1' — не файл (директория?)")
            .arg(filePath));
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_files.contains(filePath)) {
        AppLogger::instance().warning(
            QStringLiteral("FileMonitor::addFile: '%1' уже наблюдается")
            .arg(filePath));
        return false;
    }

    m_files.append(filePath);
    AppLogger::instance().info(
        QStringLiteral("Добавлен в мониторинг: '%1'").arg(filePath));
    return true;
}

bool FileMonitor::removeFile(const QString& filePath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const int idx = m_files.indexOf(filePath);
    if (idx < 0) { return false; }

    m_files.removeAt(idx);
    for (auto& checker : m_checkers) {
        checker->reset(filePath);
    }

    AppLogger::instance().info(
        QStringLiteral("Удалён из мониторинга: '%1'").arg(filePath));

    if (m_files.isEmpty()) {
        emit watchListBecameEmpty();
    }

    return true;
}

QVector<QString> FileMonitor::watchedFiles() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_files;
}

bool FileMonitor::isEmpty() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_files.isEmpty();
}

void FileMonitor::pollOnce() {
    std::lock_guard<std::mutex> lock(m_mutex);

    for (const QString& path : m_files) {
        for (auto& checker : m_checkers) {
            CheckResult result = checker->check(path);

            if (result.event == CheckEvent::NONE) {
                continue;
            }

            emit fileEventDetected(result);

            if (m_notifier != nullptr) {
                m_notifier->notify(result);
            }
        }
    }
}