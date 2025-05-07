#include "CacheReadWrite/CacheRW.h"
#include "DBConstruction/FileRead.h"
#include "KeyAndSort/KeySort.h"

int main(){
    std::cout << "starting read..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    Database* db = Foldirs::InitializeDatabase();
    Foldirs::ReadDirectory(db, "/Library");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "done reading" << std::endl;

    //test.PrintDatabase();
    std::cout << "Files stored: ";
    std::cout << "Time Reading Files: " << elapsed.count() << std::endl;

    Database* temp = db;
    std::cout << "sorting data" << std::endl;
    SortDB(fileType, temp);

    auto result = CacheRW::ReadCacheMetaData<std::string, Entry>("dbcachetest.bin");
    if(result.has_value()){
        CacheRW::CacheDB temp = *result;
        std::cout << "Key amount: "<< temp.keyamt << std::endl;
        for(int i = 0; i < temp.keyamt; i++)
           std::cout << temp.keys[i];
        std::cout << std::endl << std::endl << temp.keys[0];
        std::cout << " Entries: ";
        auto result2 = CacheRW::ReadKeyValues<Entry>(temp.keys[0], "dbcachetest.bin");
        if(result2){
            Entry* entries = *result2;
            for(int i = 0; i < temp.keys[0].count; i++)
                std::cout << entries[i].FileName << ", ";
        }
    }
    else{
        std::cerr << "failled to open cache file\n";
    }
    std::cout << std::endl;
}