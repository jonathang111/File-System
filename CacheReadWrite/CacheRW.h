#pragma once
#ifndef CACHERW_H
#define CACHERW_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <optional>
#include <cstring>

#define MAX_KEYSIZE 32
#define BLOCK_SIZE 4096
#define GROWTH_FACTOR 1.5
#define MAX_CACHESIZE 32
#define MAP_TAG "MC3"
#define VECTOR_PAIR_TAG "VP1"
#define VERSION 0.05
#define DATE 00000

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

struct CacheMeta{
    char tag[4];
    char cacheName[MAX_CACHESIZE];
    int keyamt;
    int entryamt; 
    int date;
    float version;
    int footerStart;

    CacheMeta(CacheHeader meta, const char* fileName)
    : keyamt(meta.keyamt), entryamt(meta.entryamt), date(meta.date)
    , version(meta.version), footerStart(meta.footerStart){
        strcpy(tag, meta.tag);
        strcpy(cacheName, fileName);
        cacheName[MAX_CACHESIZE-1] = '\0';
    }
};


namespace Internal{
    template<typename T>
    struct is_map;
    
    template<typename T>
    struct is_vector;

    template<typename T>
    struct is_raw_pointer;

    template<typename K, typename V>
    void StoreVectorPair(std::vector<K,V>, const char*); //distinguish between vector pair vs lone vector for loadunorderdmap internal
    
    template<typename K, typename V>
    void StoreUnorderedMap(std::unordered_map<K, V>, const char*);
}


auto GetFooterArray(CacheMeta cacheMetaData)
    -> std::optional<KeyIndexEntry*>;

template<typename Structure>
void StoreToCache(Structure loadingStruct, const char* cacheFile){
    if constexpr(CacheRW::Internal::is_map<Structure>::value)
        CacheRW::Internal::StoreUnorderedMap(loadingStruct, cacheFile);
    else if constexpr(CacheRW::Internal::is_vector<Structure>::value)
        CacheRW::Internal::StoreVectorPair(loadingStruct, cacheFile);
    else
        std::cout << "Unsupported loading structure\n";

}

template<typename keyType, typename valueType>
auto ReadCacheMetaData(const char* cacheFile)
    -> std::optional<CacheMeta> { //optional returns pointer to cacheDB or nothing.
    CacheHeader header;
    KeyIndexEntry keyindex;

    std::ifstream file(cacheFile, std::ios::binary);
    if(file.is_open()){
        file.read(reinterpret_cast<char*>(&header), sizeof(CacheHeader)); //read header
        CacheMeta output(header, cacheFile);

        file.close();
        return output;
    }
    return {};
}

template<typename returnType> //ensure free values
auto ReadKeyValues(CacheMeta cacheMetaData, KeyIndexEntry keyIndex) //use constexpr to create option CacheDB input or const char* input
    -> std::optional<returnType*> {
    if(keyIndex.count <= 0){
        std::cout << "Invalid key index\n";
        return {};
    }
    returnType* values = new returnType[keyIndex.count];
    std::ifstream file(cacheMetaData.cacheName, std::ios::binary);
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
    std::cout << "Could not open " << cacheMetaData.cacheName << std::endl;
    return {};

} 

std::ostream& operator<<(std::ostream& os, const KeyIndexEntry& entry);
}

#endif