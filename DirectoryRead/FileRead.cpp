#include "FileRead.h"
//Main functions
Database* Foldirs::InitalizeDatabase(){
    Database* db = new Database[MAX_DBCHUNK];
    ClearChunk(db, 0);
    //EntryRoom(db->filedir);
    db->currentSize = 0;
    db->maxSize = 1024;
    return db;
}

void Foldirs::ReadDirectory(Database*& db, const char* dirname){
    DIR* Directory = opendir(dirname);
    if(Directory == nullptr){
        std::cout << "Directory Error: " << errno << std::endl;
        return;
    }
    //std::cout << "successful directory open...\n";
    struct dirent* file = readdir(Directory);
    const char* filename = file->d_name;
    while(file){
        if(REDUNFILES(file->d_name)){
            file = readdir(Directory);
            continue;
        }
        AddToDB(db, dirname, file->d_name);
        file = readdir(Directory);
    }
    closedir(Directory); 
}

void Foldirs::AddToDB(Database*& db, const char* dirname, const char* filename){
    int currentsize = db->currentSize;
    if(currentsize == db->maxSize){
        IncreaseDB(db);
    }

    struct stat info;
    int error;
    char* path = new char[MAX_PATHSIZE];

    error = AppendPath(dirname, filename, path);
    if(error){
        std::cout << "Append error\n";
        return;
    }
    //std::cout << "Opening: " << path << std::endl;
    if(stat(path, &info) == 0){
        if(S_ISREG(info.st_mode)){ //only copy files
            db[currentsize].type = file;
            strcpy(db[currentsize].filedir.Directory, dirname);
            strcpy(db[currentsize].filedir.FileName, filename);
            db->currentSize++;
        }
        else if(S_ISDIR(info.st_mode))ReadDirectory(db, path);
        else db[currentsize].type = unknown;
    }
    else{
        std::cout << "Stat Error: " << errno << std::endl;
        std::cout << "Path Name: " << path << std::endl;
    }
    delete[] path;
}


//AUX functions
void Foldirs::IncreaseDB(Database* &db){
    Database* temp = new Database[db->currentSize + MAX_DBCHUNK];
    ClearChunk(temp, db->currentSize);
    memcpy(temp, db, (sizeof(Database) * db->currentSize));
    temp->currentSize = db->currentSize;
    temp->maxSize = db->maxSize + MAX_DBCHUNK;
    db = temp;
}

int Foldirs::AppendPath(const char* dir, const char* filename, char* out){ //need to add error handling
    size_t len = 0;
    if(dir && *dir){ 
        /*dir, pointers returns false if null, 
        *dir dereferenced, if empty, i.e. no char in first positoin, return false*/
        len = std::snprintf(out, MAX_PATHSIZE, "%s", dir); // copy to buff
        if(out[len-1] != '/' && len + 1 < MAX_PATHSIZE){
            out[len++] = '/';
            out[len] = '\0';
        }

        if(filename && *filename && len + strlen(filename) < MAX_PATHSIZE){
            std::strncat(out, filename, MAX_PATHSIZE - len -1); //cat to buff,
        }
        else return 1;
    }
    else return 1;

    return 0;
}

void Foldirs::ClearChunk(Database*& input, int size){
    for(int i = 0; i < size + MAX_DBCHUNK; i++){
        input[i].currentSize = 0;
        input[i].maxSize = 0;
        input[i].type = unknown;
    }
}

//Public functions
void Foldirs::PrintSize(){
    std::cout << db->currentSize << "\n";
}

void Foldirs::PrintDatabase(){
    for(int i = 0; i < db->currentSize; i++){
        std::cout << "File Type: " << db[i].type << ' ';
        std::cout << db[i].filedir.Directory << '/' << db[i].filedir.FileName << '\n';
    }
}
//void Foldirs::EntryRoom(Entry& entry){
//    entry.Directory = new char[MAX_DIRECTORY];
//    entry.FileName = new char[MAX_FILENAME];
//}
