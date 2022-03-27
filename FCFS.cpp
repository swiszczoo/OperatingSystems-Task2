#include "FCFS.h"

#include <cassert>

FCFS::FCFS() noexcept
    : _current(nullptr)
{
}

void FCFS::OnNewRequest(const TestCase::Request& request)
{
    if (_current) {
        _list.push_back(&request);
    }
    else {
        _current = &request;
        this->MoveHeadTo(_current->blockId);
    }
}

void FCFS::OnHeadStopped()
{
    assert(_current != nullptr);

    this->MarkRequestAsDone(_current->id);

    if (!_list.empty()) {
        _current = _list.front();
        _list.pop_front();
        this->MoveHeadTo(_current->blockId);
    }
    else {
        _current = nullptr;
    }
}
