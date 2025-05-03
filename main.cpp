#include "DBConstruction/FileRead.h"

int main(){
    std::cout << "starting read..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    Foldirs test("/Library");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "done reading" << std::endl;
    test.PrintDatabase();
    std::cout << "Files stored: ";
    test.PrintSize();
    std::cout << "Time Reading Files: " << elapsed.count() << std::endl;

    
}