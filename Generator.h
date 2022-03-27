#pragma once
#include "TestCase.h"

#include <memory>

class Generator {
public:
    explicit Generator() noexcept;
    ~Generator();

    void SetDiskSize(int blocks);
    int GetDiskSize() const;

    void SetTestCaseLength(int ticks);
    int GetTestCaseLength() const;

    void SetRequestCount(int min, int max);
    int GetMinRequestCount() const;
    int GetMaxRequestCount() const;

    void SetRealtimeRequestCount(int count);
    int GetRealtimeRequestCount() const;

    const TestCase& Generate();

private:
    class impl;
    std::unique_ptr<impl> pImpl;
};
