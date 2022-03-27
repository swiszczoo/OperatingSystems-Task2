#include "FileIO.h"

#include <fstream>

using std::ifstream;
using std::ofstream;
using std::endl;

bool FileIO::SaveTestCaseToFile(const TestCase& test, const std::string& filename)
{
    ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    file << test.diskBlocks << endl;
    for (const auto& req : test.normalRequests)
    {
        file << "R " << req.time << " " << req.blockId << endl;
    }

    for (const auto& req : test.rtRequests)
    {
        file << "P " << req.deadline << " " << req.blockId << endl;
    }

    file.close();
    return true;
}

bool FileIO::LoadTestCaseFromFile(TestCase& test, const std::string& filename)
{
    ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    file >> test.diskBlocks;
    int i = 0;
    int j = 0;
    while (!file.eof()) {
        char type;
        file >> type;

        if (type == 'R') {
            TestCase::Request req = {};
            req.id = i++;
            file >> req.time >> req.blockId;
            if (!file.eof()) {
                test.normalRequests.push_back(req);
            }
        }
        else if (type == 'P') {
            TestCase::RtRequest req = {};
            req.id = j++;
            file >> req.deadline >> req.blockId;
            if (!file.eof()) {
                test.rtRequests.push_back(req);
            }
        }
    }

    file.close();
    return true;
}
