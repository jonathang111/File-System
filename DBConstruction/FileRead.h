#ifndef FILEREAD
#define FILEREAD
#include "../GeneralItems.h"

class Foldirs{
    private:
    Database* db;

    //AUX functions
    int AppendPath(const char* dir, const char* filename, char* out);
    void ClearChunk(Database*&, int);
    void IncreaseDB(Database*&);
    char const* GetExtension(const char*);
    void EntryAllocate(Database*&, int, int);
    
    //main functions
    Database* InitalizeDatabase();
    void AddToDB(Database*&, const char*, const char*);
    void ReadDirectory(Database*&, const char*);

    public:
    Foldirs(const char* rd){
        db = InitalizeDatabase();
        ReadDirectory(db, rd);
    }
    
    Database* getDatabase();
    void PrintSize();
    void PrintDatabase();
};

#endif