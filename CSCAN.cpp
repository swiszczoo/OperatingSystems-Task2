#include "CSCAN.h"

#include <cassert>

CSCAN::CSCAN() noexcept
    : _current(nullptr)
{
}

void CSCAN::OnNewRequest(const TestCase::Request& request)
{
    if (_current) {
        _list.push_back(&request);
    }
    else {
        _current = &request;
        this->MoveHeadTo(_current->blockId);
    }
}

void CSCAN::OnHeadStopped()
{
    assert(_current != nullptr);

    this->MarkRequestAsDone(_current->id);

    if (!_list.empty()) {
        auto next = FindNext(this->GetCurrentHeadPosition());

        if (next == _list.end()) {
            // Reverse to the first request
            next = FindFirst();
        }

        assert(next != _list.end());

        int blockId = (*next)->blockId;
        _current = *next;
        _list.erase(next);

        this->MoveHeadTo(blockId);
    }
    else {
        _current = nullptr;
    }
}

std::list<const TestCase::Request*>::iterator CSCAN::FindNext(int current)
{
    int next = INT_MAX;
    std::list<const TestCase::Request*>::iterator nextIt = _list.end();

    for (auto it = _list.begin(); it != _list.end(); it++) {
        const TestCase::Request*& elem = *it;

        if (elem->blockId >= current && elem->blockId < next) {
            next = elem->blockId;
            nextIt = it;
        }
    }

    return nextIt;
}

std::list<const TestCase::Request*>::iterator CSCAN::FindFirst()
{
    int first = INT_MAX;
    std::list<const TestCase::Request*>::iterator firstIt = _list.end();

    for (auto it = _list.begin(); it != _list.end(); it++) {
        const TestCase::Request*& elem = *it;

        if (elem->blockId < first) {
            first = elem->blockId;
            firstIt = it;
        }
    }

    return firstIt;
}
