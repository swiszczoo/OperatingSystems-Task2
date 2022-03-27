#pragma once
#include <vector>

struct TestCase {
    int diskBlocks;

    struct Request {
        int id;
        int time;
        int blockId;
    };

    struct RtRequest {
        int id;
        int deadline;
        int blockId;
    };

    std::vector<Request> normalRequests;
    std::vector<RtRequest> rtRequests;
};
