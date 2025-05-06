#ifndef KEYSORT
#define KEYSORT
#include "../GeneralItems.h"

typedef enum{
    lexical = 1,
    fileType = 2,
    fileSize = 3
} SortType;

struct CacheHeader{
    char* magic[4];
    int entryamt;
    int keyamt;
    int version;
    int date;
};

struct KeyIndexEntry{
    char* key; //should vary depending on cache type.
    int offset; //not realitive, should be abs offset.
    int entries;
};

void SortDB(SortType, Database*&);

template<typename KeyFunc>
auto GroupEntries(Database*& db, KeyFunc keyFn) 
    -> std::unordered_map<decltype(keyFn(db[0].filedir)), std::vector<Entry>> 
{
    using KeyType = decltype(keyFn(db[0].filedir));
    std::unordered_map<KeyType, std::vector<Entry>> temp;

    for (int i = 0; i < db->currentSize; ++i) {
        temp[keyFn(db[i].filedir)].push_back(db[i].filedir); // ← or db[i] if that's what you want
    }

    return temp;
}

std::string keytest(Entry);
//void sortMap(std::unordered_map);
#endif