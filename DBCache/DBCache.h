#ifndef DBCACHE
#define DBCACHE
#include "../GeneralItems.h"

typedef enum{
    lexical = 1,
    fileType = 2,
    fileSize = 3
} SortType;

void SortDB(SortType, Database*&);
std::unordered_map<std::string, std::vector<Entry>> MapExtensions(Database*&);
//void sortMap(std::unordered_map);
void ToPOD(Entry);
void LoadToARR();
#endif