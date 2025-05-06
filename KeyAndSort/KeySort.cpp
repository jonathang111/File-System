#include "KeySort.h"

void SortDB(SortType type, Database*& db){
    std::unordered_map<std::string, std::vector<Entry>> map;
    switch(type){
        //case lexical: 
        case fileType: map = GroupEntries(db, keytest); break;
        //case fileSize:
    };
    if(!map.empty())
        for(auto it = map.begin(); it != map.end(); it++)
            std::cout << it->first << ", ";
    std::cout << std::endl;
    LoadToCache<std::string>(db, map);
}

std::string keytest(Database db){
    return db.filedir.FileExtension;
}