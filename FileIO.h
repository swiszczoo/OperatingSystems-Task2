#pragma once
#include "TestCase.h"

#include <memory>
#include <string>

class FileIO {
public:
    static bool SaveTestCaseToFile(const TestCase& test, const std::string& filename);
    static bool LoadTestCaseFromFile(TestCase& test, const std::string& filename);
};

