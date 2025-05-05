#include "DBCache.h"

void SortDB(SortType type, Database*& db){
    std::unordered_map<std::string, std::vector<Entry>> map;
    switch(type){
        //case lexical: 
        case fileType: map = MapExtensions(db); break;
        //case fileSize:
    };
    if(!map.empty())
        for(auto it = map.begin(); it != map.end(); it++)
            std::cout << it->first << ", ";
    std::cout << std::endl;
}

std::unordered_map<std::string, std::vector<Entry>> MapExtensions(Database*& db){
    std::unordered_map<std::string, std::vector<Entry>> temp;
    std::string fileExtension; //need some way to read the entrie char* and hash, 
    for(int i = 0; i < db->currentSize; i++){
        fileExtension = db[i].filedir.FileExtension;
        temp[db[i].filedir.FileExtension].push_back(db[0].filedir);
    }
    
    return temp;
}