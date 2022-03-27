#include "SCAN.h"

#include <cassert>

SCAN::SCAN() noexcept
    : _current(nullptr)
    , _movingUp(false)
{
}

void SCAN::OnNewRequest(const TestCase::Request& request)
{
    if (_current) {
        _list.push_back(&request);
    }
    else {
        _current = &request;
        this->MoveHeadTo(_current->blockId);
    }
}

void SCAN::OnHeadStopped()
{
    assert(_current != nullptr);

    this->MarkRequestAsDone(_current->id);

    if (!_list.empty()) {
        auto [prev, next] = FindNextAndPrevious(this->GetCurrentHeadPosition());
        if (_movingUp) {
            if (next != _list.end()) {
                int blockId = (*next)->blockId;
                _current = *next;
                _list.erase(next);

                this->MoveHeadTo(blockId);
            }
            else if (prev != _list.end()) {
                // No more requests, reverse the head
                int blockId = (*prev)->blockId;
                _current = *prev;
                _list.erase(prev);

                this->MoveHeadTo(blockId);
            }
            else assert(false);
        }
        else {
            if (prev != _list.end()) {
                int blockId = (*prev)->blockId;
                _current = *prev;
                _list.erase(prev);

                this->MoveHeadTo(blockId);
            }
            else if (next != _list.end()) {
                // No more requests, reverse the head
                int blockId = (*next)->blockId;
                _current = *next;
                _list.erase(next);

                this->MoveHeadTo(blockId);
            }
            else assert(false);
        }
    }
    else {
        _current = nullptr;
    }
}

void SCAN::MoveHeadTo(int block) 
{
    if (block > this->GetCurrentHeadPosition()) {
        _movingUp = true;
    }
    else if (block < this->GetCurrentHeadPosition()) {
        _movingUp = false;
    }

    DiskAlgorithm::MoveHeadTo(block);
}

std::pair<std::list<const TestCase::Request*>::iterator, std::list<const TestCase::Request*>::iterator> 
SCAN::FindNextAndPrevious(int current)
{
    int next = INT_MAX;
    int prev = -1;
    std::list<const TestCase::Request*>::iterator nextIt = _list.end();
    std::list<const TestCase::Request*>::iterator prevIt = _list.end();

    for (auto it = _list.begin(); it != _list.end(); it++) {
        const TestCase::Request*& elem = *it;

        if (elem->blockId >= current && elem->blockId < next) {
            next = elem->blockId;
            nextIt = it;
        }
        if (elem->blockId <= current && elem->blockId > prev) {
            prev = elem->blockId;
            prevIt = it;
        }
    }

    return std::make_pair(prevIt, nextIt);
}
