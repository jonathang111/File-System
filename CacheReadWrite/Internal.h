#pragma once
#include "CacheRW.h"

namespace CacheRW::Internal{

    template<typename T>
    struct is_map : std::false_type {};

    template<typename K, typename V>
    struct is_map<std::unordered_map<K,V>> : std::true_type{};
    
    template<typename T>
    struct is_vector : std::false_type{};

    template<typename V>
    struct is_vector<std::vector<V>> : std::true_type{};

    template<typename T>
    struct is_raw_pointer : std::false_type{};

    template<typename V>
    struct is_raw_pointer<V*> : std::true_type{};

    size_t ComputeOffset(size_t EntryEnd);
    
    void UpdateKeyEntries(KeyIndexEntry*& keyIndex);

    template<typename V>
    size_t StoreEntries(V entry, size_t offset, std::ofstream& cacheFile){
        size_t currentoffset = offset;
        using ValueType = typename V::value_type;
        cacheFile.seekp(currentoffset);

        if constexpr(is_vector<V>::value){
            cacheFile.write(reinterpret_cast<const char*>(entry.data()), sizeof(ValueType) * entry.size());
        }
        else if constexpr(is_raw_pointer<V>::value){
            cacheFile.write(reinterpret_cast<const char*>(entry), sizeof(ValueType));   
        }

        currentoffset = cacheFile.tellp();
        return currentoffset;
    }

    
    template <typename K>
    void UpdateKeyEntries(KeyIndexEntry& keyIndex, const K& key, size_t offset, int count){
        if constexpr (std::is_same<K, char>::value){
            keyIndex.key[0] = key;
            keyIndex.key[1] = '\0';
        }
        else if constexpr (std::is_same<K, std::string>::value){
            strncpy(keyIndex.key, key, MAX_KEYSIZE);
            keyIndex.key[MAX_KEYSIZE-1] = '\0';
        }
        else{
            std::cout << "Cannot handle key types\n";
            return;
        }
        keyIndex.offset = offset;
        keyIndex.count = count;
    }

    template<typename K, typename V>
    void storeVectorPair(std::vector<K, V> vector, const char* inputFile){
        std::ofstream cacheFile(inputFile, std::ios::binary);
        int keycount = vector.size();
        CacheRW::CacheHeader header = {VECTOR_PAIR_TAG, keycount, 0, VERSION, DATE};
        size_t currentoffset = sizeof(CacheHeader);
    }

    template<typename K, typename V>
    void StoreUnorderedMap(std::unordered_map<K, V> map, const char* inputFile){ //must be map with vector value
        std::ofstream cacheFile(inputFile, std::ios::binary);
        if(!cacheFile.is_open()){
            std::cerr << "File " << inputFile << " could not be opened\n";
            return;
        }
        
        int keycount = map.size();
        if(keycount == 0)
            std::cerr << "Warning: storing empty map\n";

        CacheRW::CacheHeader header = {MAP_TAG, keycount, 0, VERSION, DATE};
        size_t currentoffset = sizeof(CacheHeader);
        KeyIndexEntry* keyentries = new KeyIndexEntry[keycount];
        using ValueType = typename V::value_type;

        int p = 0;

        for(auto it = map.begin(); it != map.end(); it++, p++){
            UpdateKeyEntries(keyentries[p], it->first, currentoffset, it->second.size());
            header.entryamt += it->second.size();
            currentoffset = StoreEntries(it->second, currentoffset, cacheFile);
        }

        header.footerStart = ComputeOffset(currentoffset);
        cacheFile.seekp(0);
        cacheFile.write(reinterpret_cast<const char*>(&header), sizeof(CacheHeader));
        cacheFile.seekp(header.footerStart);
        cacheFile.write(reinterpret_cast<const char*>(keyentries), keycount * sizeof(KeyIndexEntry));
        cacheFile.close();
            
        delete[] keyentries;
    }

}