#pragma once
#include "DiskAlgorithm.h"

#include <list>
#include <utility>

class CSCAN : public DiskAlgorithm
{
public:
    explicit CSCAN() noexcept;
protected:
    virtual void OnNewRequest(const TestCase::Request& request) override;
    virtual void OnHeadStopped() override;
    std::list<const TestCase::Request*> _list;

private:
    const TestCase::Request* _current;

    std::list<const TestCase::Request*>::iterator FindNext(int current);
    std::list<const TestCase::Request*>::iterator FindFirst();
};

