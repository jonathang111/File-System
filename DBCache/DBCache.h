#ifndef DBCACHE
#define DBCACHE
#include "../GeneralItems.h"

typedef enum{
    lexical = 1,
    fileType = 2,
    fileSize = 3
} SortType;

void SortDB(SortType, Datbase*&);
void ToPOD(Entry);
void LoadToARR();
#endif