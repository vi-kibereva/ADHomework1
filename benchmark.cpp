#include "database.hpp"
#include <chrono>
#include <iomanip>
#include <random>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <psapi.h>

long long result_sink = 0;

template<typename SolutionType>
long long runBenchmark(Database& db, const std::string& solutionName) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<size_t> emailDist(0, db.emails.size() - 1);

    long long operationSetCount = 0;

    SolutionType solution(db);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = startTime + std::chrono::seconds(10);

    while (std::chrono::high_resolution_clock::now() < endTime) {
        for (int i = 0; i < 5; ++i) {
            result_sink += solution.maxGroup().length();
        }

        std::string emailToChange = db.emails[emailDist(rng)];
        solution.changeGroupByEmail(emailToChange, "NEW-GROUP");

        for (int i = 0; i < 100; ++i) {
            result_sink += solution.maxGradeGroup().length();
        }

        operationSetCount++;
    }
    return operationSetCount;
}

size_t get_current_memory_usage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}


int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::vector<int> sizes = {100, 1000, 10000, 100000};
    const std::string sourceFilename = "students.csv";
    std::ofstream resultsFile;
    resultsFile.open("benchmark_results.csv");
    resultsFile << "Size,Solution,Operations\n";
    std::ofstream memoryFile;
    memoryFile.open("memory_results.csv");
    memoryFile << "Size,Solution,MemoryDeltaBytes\n";

    Database masterDb;
    try {
        masterDb = Database(sourceFilename);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load master database: " << e.what() << std::endl;
        resultsFile.close();
        memoryFile.close();
        return 1;
    }
    std::cout << "Master database loaded with " << masterDb.students.size() << " students." << std::endl << std::endl;

    for (int size : sizes) {
        std::cout << "--- Benchmarking with " << size << " students ---\n";
        Database tempDb;
        tempDb.students.assign(masterDb.students.begin(), masterDb.students.begin() + size);
        tempDb.emails.assign(masterDb.emails.begin(), masterDb.emails.begin() + size);

        std::cout << "  Running performance benchmarks (10 seconds)..." << std::endl;
        try {
            long long vectorOps = runBenchmark<SolutionVector>(tempDb, "SolutionVector");
            long long mapOps = runBenchmark<SolutionMap>(tempDb, "SolutionMap");
            long long setOps = runBenchmark<SolutionSet>(tempDb, "SolutionSet");

            std::cout << "\n  Performance Results:" << std::endl;
            std::cout << "  - SolutionVector: " << vectorOps << " operation sets" << std::endl;
            std::cout << "  - SolutionMap:    " << mapOps << " operation sets" << std::endl;
            std::cout << "  - SolutionSet:    " << setOps << " operation sets" << std::endl;
            resultsFile << tempDb.students.size() << ",Vector," << vectorOps << "\n";
            resultsFile << tempDb.students.size() << ",Map," << mapOps << "\n";
            resultsFile << tempDb.students.size() << ",Set," << setOps << "\n";

        } catch (const std::exception& e) {
            std::cerr << "An error occurred during performance benchmark: " << e.what() << std::endl;
        }

        std::cout << "\n  Running memory benchmarks (delta)..." << std::endl;
        size_t memDelta = 0;

        size_t beforeVec = get_current_memory_usage();
        { SolutionVector solutionVec(tempDb); size_t afterVec = get_current_memory_usage(); memDelta = (afterVec - beforeVec); }
        std::cout << "  - SolutionVector: " << memDelta << " bytes" << std::endl;
        memoryFile << size << ",Vector," << memDelta << "\n";

        size_t beforeMap = get_current_memory_usage();
        { SolutionMap solutionMap(tempDb); size_t afterMap = get_current_memory_usage(); memDelta = (afterMap - beforeMap); }
        std::cout << "  - SolutionMap:    " << memDelta << " bytes" << std::endl;
        memoryFile << size << ",Map," << memDelta << "\n";

        size_t beforeSet = get_current_memory_usage();
        { SolutionSet solutionSet(tempDb); size_t afterSet = get_current_memory_usage(); memDelta = (afterSet - beforeSet); }
        std::cout << "  - SolutionSet:    " << memDelta << " bytes" << std::endl;
        memoryFile << size << ",Set," << memDelta << "\n";

        std::cout << "--------------------------------------------------\n" << std::endl;
    }

    resultsFile.close();
    memoryFile.close();
    std::cout << "Benchmark results saved to 'benchmark_results.csv'" << std::endl;
    std::cout << "Memory results saved to 'memory_results.csv'" << std::endl;


    std::cout << "(Final sink value: " << result_sink << ")" << std::endl;

    return 0;
}