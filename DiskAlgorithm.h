#pragma once
#include "TestCase.h"

#include <list>
#include <set>

using std::list;
using std::set;

typedef std::list<const TestCase::RtRequest*>::const_iterator RtIt;

class DiskAlgorithm {
public:
    explicit DiskAlgorithm() noexcept;

    int RunTestCase(const TestCase& test);
    int GetFailedRealtimeRequests() const;

protected:
    int GetCurrentTick() const;
    int GetCurrentHeadPosition() const;
    int GetCurrentHeadTarget() const;
    bool IsHeadMovingDown() const;
    bool IsHeadMovingUp() const;
    void MoveHeadTo(int block);
    void MarkRequestAsDone(int reqId);

    bool HasNextRealtimeRequest() const;
    const TestCase::RtRequest& GetNextRealtimeRequest();
    void SucceedRealtimeRequest(RtIt* it = nullptr);
    void FailRealtimeRequest(RtIt* it = nullptr);
    RtIt RtBegin() const;
    RtIt RtEnd() const;

    int CalcAccessTime(int blockId);

    virtual void StartSimulation();
    virtual void OnNewRequest(const TestCase::Request& request) = 0;
    virtual void OnHeadStopped() = 0;

private:
    static const double HEAD_SPEED;

    set<int> _finishedTasks;
    list<const TestCase::RtRequest*> _rtQueue;

    int _tick;
    int _headPos;
    int _headTarget;

    int _failedRtRequests;
};
