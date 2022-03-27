#pragma once
#include "DiskAlgorithm.h"

#include <list>

class FCFS : public DiskAlgorithm {
public:
    explicit FCFS() noexcept;
protected:
    virtual void OnNewRequest(const TestCase::Request& request) override;
    virtual void OnHeadStopped() override;
    std::list<const TestCase::Request*> _list;

private:
    const TestCase::Request* _current;
};
