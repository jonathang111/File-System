#ifndef KEYSORT
#define KEYSORT
#include "../GeneralItems.h"
#include "../CacheReadWrite/CacheRW.h"

typedef enum{
    lexical = 1,
    fileType = 2,
    fileSize = 3
} SortType;

void SortDB(SortType, Database*&);

template<typename KeyFunc>
auto GroupEntries(Database*& db, KeyFunc keyFn) 
    -> std::unordered_map<decltype(keyFn(db[0])), std::vector<Entry>> 
{
    using KeyType = decltype(keyFn(db[0]));
    std::unordered_map<KeyType, std::vector<Entry>> temp;

    for (int i = 0; i < db->currentSize; ++i) {
        temp[keyFn(db[i])].push_back(db[i].filedir);
    }

    return temp;
}

std::string keytest(Database);
//void sortMap(std::unordered_map);
#endif