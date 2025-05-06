#pragma once
#ifndef CACHERW_H
#define CACHERW_H

#include "../GeneralItems.h"
namespace CacheRW{

struct CacheHeader{
    char tag[4];
    int keyamt;
    int entryamt;
    float version;
    int date;
};

struct KeyIndexEntry{
    char key[MAX_KEYSIZE]; //should vary depending on cache type.
    int offset; //not realitive, should be abs offset.
    int count;
};

struct CacheDB{
    char tag[4];
    int keyamt;
    int entryamt; 
    int date;
    float version;
    KeyIndexEntry* keys;

    CacheDB(CacheHeader meta)
    : keyamt(meta.keyamt), entryamt(meta.entryamt), date(meta.date), version(meta.version){
        keys = new KeyIndexEntry[keyamt];
        strcpy(tag, meta.tag);
    }
};

template<typename mapType>
void LoadToCache(Database*& db, std::unordered_map<mapType, std::vector<Entry>> map){
    int keycount = map.size();
    std::ofstream file("dbcachetest.bin", std::ios::binary);
    CacheHeader header = {"CT2", keycount, 0, 0.2, 05062025};
    size_t currentoffset = sizeof(CacheHeader) + sizeof(KeyIndexEntry) * keycount;
    KeyIndexEntry keyentries[map.size()];

    if(file.is_open()){
        int p = 0;
        for(auto it = map.begin(); it != map.end(); it++){
            strncpy(keyentries[p].key, it->first.c_str(), MAX_KEYSIZE);
            keyentries[p].key[MAX_KEYSIZE - 1] = '\0';
            keyentries[p].offset = currentoffset;
            keyentries[p].count = it->second.size();
            header.entryamt += it->second.size();
            for(int i = 0; i < it->second.size(); i++){
                file.write(reinterpret_cast<const char*>(&it->second[i]), sizeof(Entry));
                currentoffset += sizeof(Entry);
            }
            p++;
        }
    }

    file.seekp(0);
    file.write(reinterpret_cast<const char*>(&header), sizeof(CacheHeader));
    file.seekp(sizeof(CacheHeader), std::ios::beg);
    
    for(int i = 0; i < keycount; i++){
        file.write(reinterpret_cast<const char*>(&keyentries[i]), keycount * sizeof(KeyIndexEntry));
    }
    file.close();
}

template<typename mapType>
std::optional<CacheDB> ReadCacheMetaData(/*place choice to enter bin file*/){ //optional returns pointer to cacheDB or nothing.
    CacheHeader header;
    KeyIndexEntry keyindex;
    std::unordered_map<mapType, std::vector<Entry>> map;

    std::ifstream file("dbcachetest.bin", std::ios::binary);
    if(file.is_open()){
        file.read(reinterpret_cast<char*>(&header), sizeof(CacheHeader)); //read header
        CacheDB output(header);

        for(int i = 0; i < header.keyamt; i++){
            file.read(reinterpret_cast<char*>(&keyindex), sizeof(KeyIndexEntry)); //read all key indecies
            output.keys[i] = keyindex;
        }

        file.close();
        return output;
    }
    return {};
}

std::ostream& operator<<(std::ostream& os, const KeyIndexEntry& entry);

}

#endif