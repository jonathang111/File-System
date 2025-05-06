#ifndef CACHERW_H
#define CACHERW_H

#include "../GeneralItems.h"

struct CacheHeader{
    char magic[4];
    int entryamt;
    int keyamt;
    int version;
    int date;
};

struct KeyIndexEntry{
    char key[MAX_KEYSIZE]; //should vary depending on cache type.
    int offset; //not realitive, should be abs offset.
    int count;
};

template<typename mapType>
void LoadToCache(Database*& db, std::unordered_map<mapType, std::vector<Entry>> map){
    int keycount = map.size();
    std::ofstream file("dbcachetest.bin", std::ios::binary);
    CacheHeader header = {"CT2", 100, keycount, 1, 05062025};
    size_t currentoffset = sizeof(CacheHeader) + sizeof(KeyIndexEntry) * keycount;
    KeyIndexEntry keyentries[map.size()];

    if(file.is_open()){
        file.write(reinterpret_cast<const char*>(&header), sizeof(CacheHeader));
        int p = 0;
        for(auto it = map.begin(); it != map.end(); it++){
            strncpy(keyentries[p].key, it->first.c_str(), MAX_KEYSIZE);
            keyentries[p].key[MAX_KEYSIZE - 1] = '\0';
            keyentries[p].offset = currentoffset;
            keyentries[p].count = it->second.size();
            for(int i = 0; i < it->second.size(); i++){
                file.write(reinterpret_cast<const char*>(&it->second[i]), sizeof(Entry));
                currentoffset += sizeof(Entry);
            }
            p++;
        }
    }
    file.seekp(sizeof(CacheHeader), std::ios::beg);
    for(int i = 0; i < keycount; i++){
        file.write(reinterpret_cast<const char*>(&keyentries[i]), keycount * sizeof(KeyIndexEntry));
    }
    file.close();
}

template<typename mapType>
void ReadFromCache(Database*& db){
    CacheHeader header;
    KeyIndexEntry keyindex;
    std::unordered_map<mapType, std::vector<Entry>> map;
    std::ifstream file("dbcachetest.bin", std::ios::binary);
    if(file.is_open()){
        file.read(reinterpret_cast<char*>(&header), sizeof(CacheHeader)); //read header
        std::cout << header.magic << ' ' << header.entryamt << ' ' << header.keyamt << std::endl;

        for(int i = 0; i < header.keyamt; i++){
            file.read(reinterpret_cast<char*>(&keyindex), sizeof(KeyIndexEntry)); //read all key indecies
            std::cout << keyindex.key << " at offset: " << keyindex.offset << ", ";
        }
        std::cout << std::endl;
    }   
    file.close();
}

#endif