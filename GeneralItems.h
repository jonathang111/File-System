 //this file should store structs, enums, etc that will be generally common
#ifndef GENERALITEMS
#define GENERALITEMS

#include <chrono>
#include <cstring>
//#include <filesystem> //maybe for winodws?
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>

//may have to adjust the char sizes once expanding to root dir start.
#define MAX_PATHSIZE 512
#define MAX_DIRECTORY 256
#define MAX_FILENAME 128
#define MAX_DBCHUNK 1024
#define MAX_EXTENSION 32

#define REDUNFILES(x) strcmp(x, ".") == 0 || strcmp(x, "..") == 0

typedef enum{
    unknown = 0,
    file = 1,
    folder = 2
} FileType;

struct Entry{
    char* Directory;
    char* FileName;
    char* FileExtension;
};

struct Database{
    FileType type; //type
    Entry filedir; //directory
    int maxSize;
    int currentSize;
};
#endif