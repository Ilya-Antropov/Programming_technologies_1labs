#include <QString>
#include <QFileInfo>
#include <algorithm>
#include "FileMonitor.h"

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