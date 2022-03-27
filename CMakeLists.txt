cmake_minimum_required (VERSION 3.8)

project ("SO2")
set(CMAKE_CXX_STANDARD 17)

add_executable (
	SO2 
	"SO2.cpp" 
	"Generator.cpp" 
	"FileIO.cpp" 
	"DiskAlgorithm.cpp" 
	"FCFS.cpp" 
	"SSTF.cpp" 
	"SCAN.cpp" 
	"CSCAN.cpp"
)

