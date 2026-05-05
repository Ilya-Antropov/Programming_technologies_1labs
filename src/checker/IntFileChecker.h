#pragma once

#include <QString>

enum class CheckEvent {
    NONE,
    FILE_EXISTS,
    FILE_CHANGED,
    FILE_DELETED,
    FILE_RESTORED
};
