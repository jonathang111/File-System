#include "CacheReadWrite/CacheRW.h"
#include "DBConstruction/FileRead.h"
#include "KeyAndSort/KeySort.h"

int main(){
    std::cout << "starting read..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    Foldirs test("/lib");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "done reading" << std::endl;
    //test.PrintDatabase();
    std::cout << "Files stored: ";
    test.PrintSize();
    std::cout << "Time Reading Files: " << elapsed.count() << std::endl;
    Database* temp = test.getDatabase();

    std::cout << "sorting data" << std::endl;
    SortDB(fileType, temp);

    auto result = CacheRW::ReadCacheMetaData<std::string>();
    if(result){
        CacheRW::CacheDB temp = *result;
            std::cout << "Key amount: "<< temp.keyamt << std::endl;
        for(int i = 0; i < temp.keyamt; i++)
            std::cout << temp.keys[i];
        std::cout << std::endl;
    }
    else{
        std::cerr << "failled to open cache file\n";
    }
}