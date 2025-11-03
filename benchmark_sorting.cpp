#include "database.hpp"
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>


int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::vector<int> sizes = {100, 1000, 10000, 100000};
    const std::string sourceFilename = "students.csv";

    std::ofstream resultsFile;
    resultsFile.open("sorting_results.csv");
    resultsFile << "Size,SortType,TimeMilliseconds\n";

    std::cout << "Loading master database from: " << sourceFilename << std::endl;

    Database masterDb;
    try {
        masterDb = Database(sourceFilename);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load master database: " << e.what() << std::endl;
        resultsFile.close();
        return 1;
    }

    std::cout << "Master database loaded with " << masterDb.students.size() << " students." << std::endl << std::endl;

    for (int size : sizes) {
        std::cout << "--- Benchmarking sorting with " << size << " students ---\n";

        if (masterDb.students.size() < size) {
            std::cout << "  Warning: Master database only has " << masterDb.students.size() 
                      << " students. Skipping " << size << " benchmark." << std::endl;
            std::cout << "--------------------------------------------------\n" << std::endl;
            continue;
        }

        Database unsortedDb;
        unsortedDb.students.assign(masterDb.students.begin(), masterDb.students.begin() + size);
        

        Database db_for_builtin_sort = unsortedDb;
        Database db_for_bst_sort = unsortedDb;

        auto start_builtin = std::chrono::high_resolution_clock::now();
        
        Task3::sort_built_in(db_for_builtin_sort);
        
        auto end_builtin = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration_builtin = end_builtin - start_builtin;
        
        
        auto start_bst = std::chrono::high_resolution_clock::now();
        
        Task3::bstSort(db_for_bst_sort);
        
        auto end_bst = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration_bst = end_bst - start_bst;


        std::cout << std::fixed << std::setprecision(4);
        std::cout << "  - Built-in Sort (std::sort): " << duration_builtin.count() << " ms\n";
        std::cout << "  - BST Sort: " << duration_bst.count() << " ms\n";

        resultsFile << std::fixed << std::setprecision(4);
        resultsFile << size << ",Built-in," << duration_builtin.count() << "\n";
        resultsFile << size << ",BST," << duration_bst.count() << "\n";

        std::cout << "--------------------------------------------------\n" << std::endl;
    }

    resultsFile.close();
    std::cout << "Sorting benchmark results saved to 'sorting_results.csv'" << std::endl;

    return 0;
}