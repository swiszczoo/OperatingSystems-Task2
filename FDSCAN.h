#pragma once
#include "DiskAlgorithm.h"

#include <cassert>
#include <climits>
#include <queue>
#include <type_traits>

using std::queue;

template <typename Alg>
class FDSCAN : public Alg {
    static_assert(
        std::is_base_of<DiskAlgorithm, Alg>::value,
        "Alg must be a disk algorithm!"
        );
public:
    explicit FDSCAN() noexcept
        : Alg()
        , _currentRt()
        , _currentRtValid(false)
    {
    }

protected:
    virtual void StartSimulation() override
    {
        if (this->HasNextRealtimeRequest()) {
            this->HandleNextRequest();
        }
    }

    virtual void OnNewRequest(const TestCase::Request& request) override
    {
        if (this->HasNextRealtimeRequest()) {
            _delayedRequests.push(&request);
        }
        else {
            Alg::OnNewRequest(request);
        }
    }

    virtual void OnHeadStopped() override
    {
        if (!this->HasNextRealtimeRequest()) {
            Alg::OnHeadStopped();
            return;
        }

        assert(_currentRtValid);

        if ((*_currentRt)->deadline >= this->GetCurrentTick()) {
            this->SucceedRealtimeRequest(&_currentRt);
        }
        else {
            this->FailRealtimeRequest(&_currentRt);
        }

        HandleNextRequest();

        if (!this->HasNextRealtimeRequest()) {
            while (!_delayedRequests.empty()) {
                Alg::OnNewRequest(*_delayedRequests.front());
                _delayedRequests.pop();
            }
        }
    }

private:
    RtIt _currentRt;
    bool _currentRtValid;
    queue<const TestCase::Request*> _delayedRequests;

    void HandleNextRequest() {
        _currentRtValid = false;

        while (this->HasNextRealtimeRequest()) {
            auto candidate = this->RtBegin();

            if ((*candidate)->deadline < this->CalcAccessTime((*candidate)->blockId)) {
                // It's impossible to get there in time
                this->FailRealtimeRequest(&candidate);
                continue;
            }

            // Find the nearest rt request between us and candidate

            int nearest = abs((*candidate)->blockId - this->GetCurrentHeadPosition());
            auto nearestIt = candidate;

            bool movingUp = (*candidate)->blockId > this->GetCurrentHeadPosition();

            for (auto it = candidate; it != this->RtEnd(); it++) {
                int distance = abs((*it)->blockId - this->GetCurrentHeadPosition());

                if (movingUp) {
                    if ((*it)->blockId >= this->GetCurrentHeadPosition() 
                        && (*it)->blockId <= (*candidate)->blockId
                        && distance < nearest) 
                    {
                        nearest = distance;
                        nearestIt = it;
                    }
                }
                else {
                    if ((*it)->blockId <= this->GetCurrentHeadPosition()
                        && (*it)->blockId >= (*candidate)->blockId
                        && distance < nearest) 
                    {
                        nearest = distance;
                        nearestIt = it;
                    }
                }
            }

            _currentRt = nearestIt;
            _currentRtValid = true;
            this->MoveHeadTo((*_currentRt)->blockId);
            break;
        }
    }
};
