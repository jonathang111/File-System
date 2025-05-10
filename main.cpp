#include "CacheReadWrite/CacheRW.h"
#include "CacheReadWrite/Internal.h"
#include "DBConstruction/FileRead.h"
#include "KeyAndSort/KeySort.h"

int main(){
    std::cout << "starting read..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    Database* db = Foldirs::InitializeDatabase();
    Foldirs::ReadDirectory(db, "/lib");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "done reading" << std::endl;

    //test.PrintDatabase();
    std::cout << "Files stored: ";
    Foldirs::PrintSize(db);
    std::cout << "Time Reading Files: " << elapsed.count() << std::endl;

    Database* temp = db;
    std::cout << "sorting data" << std::endl;
    std::unordered_map<char, std::vector<Entry>> map = KeySort::mapDB<char, Entry>(KeySort::lexical, temp);
    CacheRW::StoreToCache(map, "dbcachetest.bin");

    auto result = CacheRW::ReadCacheMetaData<char, Entry>("dbcachetest.bin");
    if(result.has_value()){
        CacheRW::CacheMeta cacheMetaData = *result;
        auto result2 = CacheRW::GetFooterArray(cacheMetaData);
        if(result2.has_value()){
            CacheRW::KeyIndexEntry* keyIndex = *result2;
            for(int i = 0; i < cacheMetaData.keyamt; i++)
                std::cout << keyIndex[i] << ", ";
            std::cout << std::endl;

            auto values = CacheRW::ReadKeyValues<Entry>(cacheMetaData, keyIndex[0]);
            if(values.has_value()){
                Entry* keyVals = *values;
                for(int i = 0; i < keyIndex[0].count; i++)
                    std::cout << keyVals[i].FileName << ", ";
            }
        }
    }
    else
        std::cerr << "failled to open cache file\n";
    
    std::cout << std::endl;
}