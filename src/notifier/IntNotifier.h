#pragma once

#include <QString>
#include "../checker/IntFileChecker.h"

class IntNotifier {
public:
    virtual ~IntNotifier() = default;

    virtual void notify(const struct CheckResult& result) = 0;
};