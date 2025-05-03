#ifndef FILEREAD
#define FILEREAD

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

#define REDUNFILES(x) strcmp(x, ".") == 0 || strcmp(x, "..") == 0

typedef enum{
    unknown = 0,
    file = 1,
    folder = 2
} FileType;

struct Entry{
    char* Directory;
    char* FileName;

    Entry(){
        Directory = new char[MAX_DIRECTORY];
        FileName = new char[MAX_FILENAME];
    }
};

struct Database{
    FileType type; //type
    Entry filedir; //directory
    int maxSize;
    int currentSize;
};

//implement a class with these functions.

class Foldirs{
    private:
    Database* db;

    int AppendPath(const char* dir, const char* filename, char* out);
    void ClearChunk(Database*&, int);
    //void EntryRoom(Entry&);

    Database* InitalizeDatabase();
    void AddToDB(Database*&, const char*, const char*);
    void IncreaseDB(Database*&);
    void ReadDirectory(Database*&, const char*);

    public:
    Foldirs(const char* rd){
        db = InitalizeDatabase();
        ReadDirectory(db, rd);
    }
    
    void PrintSize();
    void PrintDatabase();
};

#endif