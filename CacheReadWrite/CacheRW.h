#pragma once
#ifndef CACHERW_H
#define CACHERW_H
#define MAX_KEYSIZE 20
#define BLOCK_SIZE 4096
#define GROWTH_FACTOR 1.5
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <optional>

namespace CacheRW{

struct CacheHeader{
    char tag[4];
    int keyamt;
    int entryamt;
    float version;
    int date;
    int footerStart;
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
    int footerStart;
    KeyIndexEntry* keys;

    CacheDB(CacheHeader meta)
    : keyamt(meta.keyamt), entryamt(meta.entryamt), date(meta.date)
    , version(meta.version), footerStart(meta.footerStart){
        keys = new KeyIndexEntry[keyamt];
        strcpy(tag, meta.tag);
    }
};

size_t ComputeOffset(size_t EntryEnd);

template<typename keyType, typename valueType>
void LoadToCache(std::unordered_map<keyType, std::vector<valueType>> map, const char* cacheFile){
    int keycount = map.size();
    std::ofstream file(cacheFile, std::ios::binary);
    CacheHeader header = {"CL3", keycount, 0, 0.3, 05062025, 0};
    size_t currentoffset = sizeof(CacheHeader) + sizeof(KeyIndexEntry) * keycount;
    KeyIndexEntry* keyentries = new KeyIndexEntry[keycount];

    if(file.is_open()){
        int p = 0;
        for(auto it = map.begin(); it != map.end(); it++, p++){
            file.seekp(currentoffset);
            if constexpr (std::is_same<keyType, char>::value){
                keyentries[p].key[0] = it->first;
                keyentries[p].key[1] = '\0';
            }
            else{
            strncpy(keyentries[p].key, it->first.c_str(), MAX_KEYSIZE);
            keyentries[p].key[MAX_KEYSIZE-1] = '\0';
            }

            //keyentries[p].key[MAX_KEYSIZE - 1] = '\0';
            keyentries[p].offset = currentoffset;
            keyentries[p].count = it->second.size();
            header.entryamt += it->second.size();
            for(int i = 0; i < it->second.size(); i++){
                file.write(reinterpret_cast<const char*>(&it->second[i]), sizeof(valueType));
                currentoffset += sizeof(valueType);
            }
        }
        header.footerStart = ComputeOffset(currentoffset);
        file.seekp(0);
        file.write(reinterpret_cast<const char*>(&header), sizeof(CacheHeader));
        file.seekp(header.footerStart);
        file.write(reinterpret_cast<const char*>(keyentries), keycount * sizeof(KeyIndexEntry));
        file.close();
    }
    else{
        std::cout << "Could not open " << cacheFile << std::endl;
    }
    delete[] keyentries;
}

template<typename keyType, typename valueType>
auto ReadCacheMetaData(const char* cacheFile)
    -> std::optional<CacheDB> { //optional returns pointer to cacheDB or nothing.
    CacheHeader header;
    KeyIndexEntry keyindex;
    std::unordered_map<keyType, std::vector<valueType>> map;

    std::ifstream file(cacheFile, std::ios::binary);
    if(file.is_open()){
        file.read(reinterpret_cast<char*>(&header), sizeof(CacheHeader)); //read header
        CacheDB output(header);

        file.seekg(output.footerStart);
        file.read(reinterpret_cast<char*>(output.keys), sizeof(KeyIndexEntry)*header.keyamt); //read all key indecies

        file.close();
        return output;
    }
    return {};
}

template<typename returnType> //ensure free values
auto ReadKeyValues(KeyIndexEntry keyIndex, const char* cacheFile)
    -> std::optional<returnType*> {
    if(keyIndex.count <= 0){
        std::cout << "Invalid key index\n";
        return {};
    }
    returnType* values = new returnType[keyIndex.count];
    std::ifstream file(cacheFile, std::ios::binary);
    if(file.is_open()){
        file.seekg(keyIndex.offset);
        if(!file){
            std::cout << "Bad file offset\n";
            delete[] values;
            return {};
        }
        file.read(reinterpret_cast<char*>(values), sizeof(returnType) * keyIndex.count);
        if(!file){
            std::cout << "Bad file offset\n";
            delete[] values;
            return {};
        }
        file.close();
        return values;
    }
    std::cout << "Could not open " << cacheFile << std::endl;
    return {};

} 

std::ostream& operator<<(std::ostream& os, const KeyIndexEntry& entry);
}

#endif