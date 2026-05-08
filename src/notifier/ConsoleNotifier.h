#pragma once

#include <QString>
#include "IntNotifier.h"

class ConsoleNotifier : public IntNotifier {
public:
    ConsoleNotifier() = default;

    ~ConsoleNotifier() override = default;

    void notify(const CheckResult& result) override;

private:
    static QString formatMessage(const CheckResult& result);
};