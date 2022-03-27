#pragma once
#include "DiskAlgorithm.h"

#include <queue>
#include <type_traits>

using std::queue;

template <typename Alg>
class EDF : public Alg {
    static_assert(
        std::is_base_of<DiskAlgorithm, Alg>::value,
        "Alg must be a disk algorithm!"
        );
public:
    explicit EDF() noexcept
        : Alg()
        , _currentRt(nullptr)
    {
    }

protected:
    virtual void StartSimulation() override
    {
        if (this->HasNextRealtimeRequest()) {
            _currentRt = &this->GetNextRealtimeRequest();
            this->MoveHeadTo(_currentRt->blockId);
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

        if (_currentRt->deadline >= this->GetCurrentTick()) {
            this->SucceedRealtimeRequest();
        }
        else {
            this->FailRealtimeRequest();
        }

        _currentRt = nullptr;

        while (_currentRt == nullptr) {
            if (!this->HasNextRealtimeRequest()) {
                break;
            }

            _currentRt = &this->GetNextRealtimeRequest();

            if (_currentRt->deadline < this->GetCurrentTick()) {
                this->FailRealtimeRequest();
                _currentRt = nullptr;
            }
            else {
                this->MoveHeadTo(_currentRt->blockId);
            }
        }

        if (!this->HasNextRealtimeRequest()) {
            while (!_delayedRequests.empty()) {
                Alg::OnNewRequest(*_delayedRequests.front());
                _delayedRequests.pop();
            }
        }
    }

private:
    const TestCase::RtRequest* _currentRt;
    queue<const TestCase::Request*> _delayedRequests;
};
