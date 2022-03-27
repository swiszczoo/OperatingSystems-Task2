#include "DiskAlgorithm.h"

#include <climits>
#include <queue>
#include <stdlib.h>

using std::queue;

const double DiskAlgorithm::HEAD_SPEED = 100.0;

DiskAlgorithm::DiskAlgorithm() noexcept
    : _tick(0)
    , _headPos(0)
    , _headTarget(0)
    , _failedRtRequests(0)
{
}

int DiskAlgorithm::RunTestCase(const TestCase& test)
{
    queue<const TestCase::Request*> requests;
    int totalHeadJumps = 0;

    for (const auto& req : test.normalRequests) {
        requests.push(&req);
    }

    for (const auto& req : test.rtRequests) {
        _rtQueue.push_back(&req);
    }

    this->StartSimulation();

    while (_finishedTasks.size() < test.normalRequests.size()) {
        int lastTick = _tick;

        if (!requests.empty() && _tick == requests.front()->time) {
            this->OnNewRequest(*requests.front());
            requests.pop();
        }

        int nextRequestTime = INT_MAX;
        if (!requests.empty()) {
            nextRequestTime = requests.front()->time;
        }
        int headStopTime = _tick + (int)ceil(abs((_headTarget - _headPos) / HEAD_SPEED));

        if (nextRequestTime < headStopTime || _headTarget == _headPos) {
            bool alreadyOk = _headTarget == _headPos;

            if (_headTarget > _headPos) {
                _headPos += static_cast<int>(floor((nextRequestTime - _tick) * HEAD_SPEED));
                totalHeadJumps += static_cast<int>(floor((nextRequestTime - _tick) * HEAD_SPEED));
            }
            else if (_headTarget < _headPos) {
                _headPos -= static_cast<int>(floor((nextRequestTime - _tick) * HEAD_SPEED));
                totalHeadJumps += static_cast<int>(floor((nextRequestTime - _tick) * HEAD_SPEED));
            }

            _tick = nextRequestTime;

            if (!alreadyOk && _headTarget == _headPos) {
                this->OnHeadStopped();
            }
        }
        else {
            totalHeadJumps += abs(_headPos - _headTarget);
            _headPos = _headTarget;
            _tick = headStopTime;

            this->OnHeadStopped();
        }

        if (_tick == lastTick) {
            _tick++;
        }
    }

    while (!_rtQueue.empty())
    {
        FailRealtimeRequest();
    }

    return totalHeadJumps;
}

int DiskAlgorithm::GetFailedRealtimeRequests() const
{
    return _failedRtRequests;
}

int DiskAlgorithm::GetCurrentTick() const
{
    return _tick;
}

int DiskAlgorithm::GetCurrentHeadPosition() const
{
    return _headPos;
}

int DiskAlgorithm::GetCurrentHeadTarget() const
{
    return _headTarget;
}

bool DiskAlgorithm::IsHeadMovingDown() const
{
    return _headTarget < _headPos;
}

bool DiskAlgorithm::IsHeadMovingUp() const
{
    return _headTarget > _headPos;
}

void DiskAlgorithm::MoveHeadTo(int block)
{
    _headTarget = block;

    if (_headTarget == _headPos) {
        this->OnHeadStopped();
    }
}

void DiskAlgorithm::MarkRequestAsDone(int reqId)
{
    _finishedTasks.insert(reqId);
}

bool DiskAlgorithm::HasNextRealtimeRequest() const
{
    return !_rtQueue.empty();
}

const TestCase::RtRequest& DiskAlgorithm::GetNextRealtimeRequest()
{
    return *_rtQueue.front();
}

void DiskAlgorithm::SucceedRealtimeRequest(RtIt* it)
{
    if (!it) {
        _rtQueue.pop_front();
    }
    else {
        _rtQueue.erase(*it);
    }
}

void DiskAlgorithm::FailRealtimeRequest(RtIt* it)
{
    if (!it) {
        _rtQueue.pop_front();
    }
    else {
        _rtQueue.erase(*it);
    }

    _failedRtRequests++;
}

RtIt DiskAlgorithm::RtBegin() const
{
    return _rtQueue.begin();
}

RtIt DiskAlgorithm::RtEnd() const
{
    return _rtQueue.end();
}

int DiskAlgorithm::CalcAccessTime(int blockId)
{
    return _tick + static_cast<int>(ceil(abs((blockId - _headPos) / HEAD_SPEED)));
}

void DiskAlgorithm::StartSimulation()
{
}
