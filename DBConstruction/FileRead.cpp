#include "FileRead.h"
//Main functions
Database* Foldirs::InitalizeDatabase(){
    Database* db = new Database[MAX_DBCHUNK];
    ClearChunk(db, 0);
    for(int i = 0; i < MAX_DBCHUNK; i++)
        EntryAllocate(db[i].filedir);
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
    if(currentsize == db->maxSize)
        IncreaseDB(db);
    struct stat info;
    int error;
    char* path = new char[MAX_PATHSIZE];

    error = AppendPath(dirname, filename, path);
    if(error){
        std::cout << "Append error\n";
        return;
    }
    //std::cout << "Opening: " << path << std::endl;
    if(lstat(path, &info) == 0){ //lstat instead of stat for symblink prevention
        if(S_ISREG(info.st_mode)){ //only copy files
            db[currentsize].type = file;
            strcpy(db[currentsize].filedir.Directory, dirname);
            strcpy(db[currentsize].filedir.FileName, filename);
            const char* temp = GetExtension(filename);
            strncpy(db[currentsize].filedir.FileExtension, temp, MAX_EXTENSION);//NULL of ext too long
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
void Foldirs::IncreaseDB(Database* &db){ //problem here
    Database* temp = new Database[db->currentSize + MAX_DBCHUNK];;
    Database* temp2 = db;
    ClearChunk(temp, db->currentSize);
    for(int i = db->currentSize; i < MAX_DBCHUNK + db->currentSize; i++)
        EntryAllocate(temp[i].filedir);

    memcpy(temp, db, (sizeof(Database) * db->currentSize));
    temp->currentSize = db->currentSize;
    temp->maxSize = db->maxSize + MAX_DBCHUNK;
    db = temp;
    delete[] temp2; //double free
}

int Foldirs::AppendPath(const char* dir, const char* filename, char* out){ //need to add error handling
    size_t len = 0;
    if(dir && *dir){ 
        /*dir, pointers returns false if null, 
        *dir dereferenced, if empty, i.e. no char in first position, return false*/
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

const char* Foldirs::GetExtension(const char* filename){
    const char* temp = strrchr(filename, '.');
    if(!temp || temp == filename || strlen(temp) >= MAX_EXTENSION) return "NULL";
    return temp + 1;
}

void Foldirs::EntryAllocate(Entry& entry){
    entry.Directory = new char[MAX_DIRECTORY];
    entry.FileName = new char[MAX_FILENAME];
    entry.FileExtension = new char[MAX_EXTENSION];
}

//Public functions
Database* Foldirs::getDatabase(){
    return db;
}

void Foldirs::PrintSize(){
    std::cout << db->currentSize << "\n";
}

void Foldirs::PrintDatabase(){
    for(int i = 0; i < db->currentSize; i++){
        std::cout << "File Type: " << db[i].type << ' ' << "Extension: " << db[i].filedir.FileExtension << ' ';
        std::cout << db[i].filedir.Directory << '/' << db[i].filedir.FileName << '\n';
    }
}