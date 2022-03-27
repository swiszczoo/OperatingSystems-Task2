#include "Generator.h"

#include <algorithm>
#include <ctime>
#include <random>

using std::uniform_int_distribution;
using std::mt19937_64;

class Generator::impl {
public:
    Generator::impl::impl() noexcept
        : _case({})
        , _diskBlocks(1000)
        , _tickCount(500)
        , _minRequests(50)
        , _maxRequests(100)
        , _rtRequests(0)
    {
        _eng.seed(time(NULL));
    }

    void SetDiskSize(int blocks)
    {
        _diskBlocks = blocks;
    }

    int GetDiskSize() const
    {
        return _diskBlocks;
    }

    void SetTestCaseLength(int ticks)
    {
        _tickCount = ticks;
    }

    int GetTestCaseLength() const
    {
        return _tickCount;
    }

    void SetRequestCount(int min, int max)
    {
        _minRequests = min;
        _maxRequests = max;
    }

    int GetMinRequestCount() const
    {
        return _minRequests;
    }

    int GetMaxRequestCount() const
    {
        return _maxRequests;
    }

    void SetRealtimeRequestCount(int count)
    {
        _rtRequests = count;
    }

    int GetRealtimeRequestCount() const
    {
        return _rtRequests;
    }

    const TestCase& Generate()
    {
        uniform_int_distribution<> reqDist(_minRequests, _maxRequests);
        uniform_int_distribution<> blockDist(0, _diskBlocks);
        uniform_int_distribution<> timeDist(0, _tickCount);
        int requests = reqDist(_eng);
        int rtRequests = _rtRequests;

        _case.diskBlocks = _diskBlocks;
        _case.normalRequests.clear();
        _case.rtRequests.clear();

        _case.normalRequests.reserve(requests);
        _case.rtRequests.reserve(rtRequests);

        for (; requests > 0; --requests) {
            _case.normalRequests.push_back({ 0, timeDist(_eng), blockDist(_eng) });
        }

        for (; rtRequests > 0; --rtRequests) {
            _case.rtRequests.push_back({ 0, timeDist(_eng), blockDist(_eng) });
        }

        std::stable_sort(_case.normalRequests.begin(), _case.normalRequests.end(),
            [](const TestCase::Request& a, const TestCase::Request& b) -> bool
            {
                return a.time < b.time;
            }
        );

        std::stable_sort(_case.rtRequests.begin(), _case.rtRequests.end(),
            [](const TestCase::RtRequest& a, const TestCase::RtRequest& b) -> bool
            {
                return a.deadline < b.deadline;
            }
        );

        return _case;
    }

private:
    int _diskBlocks;
    int _tickCount;
    int _minRequests;
    int _maxRequests;
    int _rtRequests;

    TestCase _case;
    std::mt19937_64 _eng;
};

Generator::Generator() noexcept
{
    pImpl = std::make_unique<impl>();
}

Generator::~Generator() = default;

void Generator::SetDiskSize(int blocks)
{
    pImpl->SetDiskSize(blocks);
}

int Generator::GetDiskSize() const
{
    return pImpl->GetDiskSize();
}

void Generator::SetTestCaseLength(int ticks)
{
    pImpl->SetTestCaseLength(ticks);
}

int Generator::GetTestCaseLength() const
{
    return pImpl->GetTestCaseLength();
}

void Generator::SetRequestCount(int min, int max)
{
    pImpl->SetRequestCount(min, max);
}

int Generator::GetMinRequestCount() const
{
    return pImpl->GetMinRequestCount();
}

int Generator::GetMaxRequestCount() const
{
    return pImpl->GetMaxRequestCount();
}

void Generator::SetRealtimeRequestCount(int count)
{
    pImpl->SetRealtimeRequestCount(count);
}

int Generator::GetRealtimeRequestCount() const
{
    return pImpl->GetRealtimeRequestCount();
}

const TestCase& Generator::Generate()
{
    return pImpl->Generate();
}
