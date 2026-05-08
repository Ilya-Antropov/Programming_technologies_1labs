#pragma once

#include <QString>

enum class CheckEvent {
    NONE,
    FILE_EXISTS,
    FILE_CHANGED,
    FILE_TOUCHED,
    FILE_DELETED,
    FILE_RESTORED
};

struct CheckResult {
    CheckEvent event;
    QString path;
    qint64 size;
    qint64 oldSize;


    explicit CheckResult(CheckEvent e = CheckEvent::NONE,
                         const QString& p = {},
                         qint64 sz = -1,
                         qint64 old = -1)
        : event(e), path(p), size(sz), oldSize(old) {
    }
};

class IntFileChecker {
public:
    virtual ~IntFileChecker() = default;

    virtual CheckResult check(const QString& filePath) = 0;

    virtual void reset(const QString& filePath) = 0;
};
