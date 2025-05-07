#include "KeySort.h"

void SortDB(SortType type, Database*& db){
    std::unordered_map<std::string, std::vector<Entry>> map;
    switch(type){
        //case lexical: 
        case fileType: map = GroupEntries(db, keytest); break;
        //case fileSize:
    };
    
    if(!map.empty())
        CacheRW::LoadToCache<std::string, Entry>(map, "dbcachetest.bin");
}

std::string keytest(Database db){
    return db.filedir.FileExtension;
}