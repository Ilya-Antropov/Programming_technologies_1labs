#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <memory>
#include <mutex>
#include "../checker/IntFileChecker.h"
#include "../notifier/IntNotifier.h"

class FileMonitor : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(FileMonitor);

public:
    explicit FileMonitor(QObject* parent = nullptr);

    ~FileMonitor() override = default;

    void addChecker(std::unique_ptr<IntFileChecker> checker);

    void setNotifier(IntNotifier* notifier);

    bool addFile(const QString& filePath);

    bool removeFile(const QString& filePath);

    bool isEmpty() const;

    QVector<QString> watchedFiles() const;

    signals:
    // будут добавлены

private:
    QVector<QString> m_files;
    QVector<std::unique_ptr<IntFileChecker> > m_checkers;
    IntNotifier* m_notifier;
    mutable std::mutex m_mutex;
};