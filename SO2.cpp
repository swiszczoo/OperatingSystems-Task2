#include "FileIO.h"
#include "Generator.h"

#include "FCFS.h"
#include "SSTF.h"
#include "SCAN.h"
#include "CSCAN.h"

#include "EDF.h"
#include "FDSCAN.h"

#include <algorithm>
#include <iostream>

using namespace std;

const int TEST_COUNT = 50;

void RunGenerator()
{
    Generator gen;
    for (int i = 0; i < TEST_COUNT; ++i) {
        gen.SetDiskSize(1000 + i * 10);
        gen.SetTestCaseLength(150);
        gen.SetRequestCount(50 + i, 100 + i);
        gen.SetRealtimeRequestCount(i);

        const std::string filename = "test" + std::to_string(i + 1) + ".txt";
        FileIO::SaveTestCaseToFile(gen.Generate(), filename);
    }
}

void RunTests()
{
    int fcfsTotal = 0;
    int sstfTotal = 0;
    int scanTotal = 0;
    int cscanTotal = 0;
    int fcfsEdfTotal = 0;
    int sstfEdfTotal = 0;
    int scanEdfTotal = 0;
    int cscanEdfTotal = 0;
    int fcfsFdScanTotal = 0;
    int sstfFdScanTotal = 0;
    int scanFdScanTotal = 0;
    int cscanFdScanTotal = 0;

    int fcfsEdfFail = 0;
    int sstfEdfFail = 0;
    int scanEdfFail = 0;
    int cscanEdfFail = 0;
    int fcfsFdScanFail = 0;
    int sstfFdScanFail = 0;
    int scanFdScanFail = 0;
    int cscanFdScanFail = 0;

    for (int i = 0; i < TEST_COUNT; ++i) {
        cout << "Running test " << i + 1 << "... ";
        TestCase tc;
        const std::string filename = "test" + std::to_string(i + 1) + ".txt";
        FileIO::LoadTestCaseFromFile(tc, filename);

        fcfsTotal += FCFS().RunTestCase(tc); cout << "FCFS ";
        sstfTotal += SSTF().RunTestCase(tc); cout << "SSTF ";
        scanTotal += SCAN().RunTestCase(tc); cout << "SCAN ";
        cscanTotal += CSCAN().RunTestCase(tc); cout << "C-SCAN ";

        EDF<FCFS> edfFcfs;
        fcfsEdfTotal += edfFcfs.RunTestCase(tc); cout << "FCFS/EDF ";
        fcfsEdfFail += edfFcfs.GetFailedRealtimeRequests();

        EDF<SSTF> edfSstf;
        sstfEdfTotal += edfSstf.RunTestCase(tc); cout << "SSTF/EDF ";
        sstfEdfFail += edfSstf.GetFailedRealtimeRequests();

        EDF<SCAN> edfScan;
        scanEdfTotal += edfScan.RunTestCase(tc); cout << "SCAN/EDF ";
        scanEdfFail += edfScan.GetFailedRealtimeRequests();

        EDF<CSCAN> edfCscan;
        cscanEdfTotal += edfCscan.RunTestCase(tc); cout << "C-SCAN/EDF ";
        cscanEdfFail += edfCscan.GetFailedRealtimeRequests();

        FDSCAN<FCFS> fdFcfs;
        fcfsFdScanTotal += fdFcfs.RunTestCase(tc); cout << "FCFS/FDSCAN ";
        fcfsFdScanFail += fdFcfs.GetFailedRealtimeRequests();

        FDSCAN<SSTF> fdSstf;
        sstfFdScanTotal += fdSstf.RunTestCase(tc); cout << "SSTF/FDSCAN ";
        sstfFdScanFail += fdSstf.GetFailedRealtimeRequests();

        FDSCAN<SCAN> fdScan;
        scanFdScanTotal += fdScan.RunTestCase(tc); cout << "SCAN/FDSCAN ";
        scanFdScanFail += fdScan.GetFailedRealtimeRequests();

        FDSCAN<CSCAN> fdCscan;
        cscanFdScanTotal += fdCscan.RunTestCase(tc); cout << "C-SCAN/FDSCAN ";
        cscanFdScanFail += fdCscan.GetFailedRealtimeRequests();

        cout << endl;
    }

    cout << endl << endl << "S T A T S " << endl;
    cout << "=====================================" << endl;
    cout << "avg FCFS : " << (fcfsTotal / (double)TEST_COUNT) << endl;
    cout << "avg SSTF : " << (sstfTotal / (double)TEST_COUNT) << endl;
    cout << "avg SCAN : " << (scanTotal / (double)TEST_COUNT) << endl;
    cout << "avg C-SCAN : " << (cscanTotal / (double)TEST_COUNT) << endl;
    cout << endl;
    cout << "avg EDF/FCFS : " << (fcfsEdfTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (fcfsEdfFail / (double)TEST_COUNT) << endl;
    cout << "avg EDF/SSTF : " << (sstfEdfTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (sstfEdfFail / (double)TEST_COUNT) << endl;
    cout << "avg EDF/SCAN : " << (scanEdfTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (scanEdfFail / (double)TEST_COUNT) << endl;
    cout << "avg EDF/C-SCAN : " << (cscanEdfTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (cscanEdfFail / (double)TEST_COUNT) << endl;
    cout << endl;
    cout << "avg FD-SCAN/FCFS : " << (fcfsFdScanTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (fcfsFdScanFail / (double)TEST_COUNT) << endl;
    cout << "avg FD-SCAN/SSTF : " << (sstfFdScanTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (sstfFdScanFail / (double)TEST_COUNT) << endl;
    cout << "avg FD-SCAN/SCAN : " << (scanFdScanTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (scanFdScanFail / (double)TEST_COUNT) << endl;
    cout << "avg FD-SCAN/C-SCAN : " << (cscanFdScanTotal / (double)TEST_COUNT) << " | avg failed rt req : " << (cscanFdScanFail / (double)TEST_COUNT) << endl;
    cout << endl;
}

int main()
{
    char mode;
    cout << "Generate or run tests? [G/T] > ";
    cin >> mode;

    if (mode == 'G' || mode == 'g') {
        RunGenerator();
        return 0;
    }
    else if (mode == 'T' || mode == 't') {
        RunTests();
        return 0;
    }

    return 1;
}
