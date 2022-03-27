#include "SSTF.h"

#include <cassert>

SSTF::SSTF() noexcept
    : _current(nullptr)
{
}

void SSTF::OnNewRequest(const TestCase::Request& request)
{
    if (_current) {
        _list.push_back(&request);
    }
    else {
        _current = &request;
        this->MoveHeadTo(_current->blockId);
    }
}

void SSTF::OnHeadStopped()
{
    assert(_current != nullptr);

    this->MarkRequestAsDone(_current->id);

    if (!_list.empty()) {
        auto next = FindMinDistance();
        int blockId = (*next)->blockId;
        _current = *next;
        _list.erase(next);

        this->MoveHeadTo(blockId);
    }
    else {
        _current = nullptr;
    }
}

std::list<const TestCase::Request*>::iterator SSTF::FindMinDistance()
{
    std::list<const TestCase::Request*>::iterator minimum = _list.begin();
    int minDist = INT_MAX;

    for (auto it = _list.begin(); it != _list.end(); it++) {
        int dist = abs(this->GetCurrentHeadPosition() - (*it)->blockId);
        if (dist < minDist) {
            minimum = it;
            minDist = dist;
        }
    }

    return minimum;
}
