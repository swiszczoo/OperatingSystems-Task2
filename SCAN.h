#pragma once
#include "DiskAlgorithm.h"

#include <list>
#include <utility>

class SCAN : public DiskAlgorithm
{
public:
    explicit SCAN() noexcept;
protected:
    virtual void OnNewRequest(const TestCase::Request& request) override;
    virtual void OnHeadStopped() override;
    void MoveHeadTo(int block);
    std::list<const TestCase::Request*> _list;

private:
    const TestCase::Request* _current;
    bool _movingUp;

    std::pair<std::list<const TestCase::Request*>::iterator, std::list<const TestCase::Request*>::iterator> FindNextAndPrevious(int current);
};

