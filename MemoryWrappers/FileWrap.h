#pragma once
#include "MemInterface.h"
#include "../GeneralItems.h"

class FileEntry : public MemInterface<Entry>{
    private:
        Entry* entry;
        size_t size;
    public:
    FileEntry(size_t size){
        if(size > 0)
            entry = new Entry[size];
        else
            entry = nullptr;
    }

    ~FileEntry(){
        delete[] entry;
    }

    Entry& operator*() const override {
        return *entry;
    }

    Entry* operator->() const override {
        return entry;
    }

    Entry& operator[](size_t index) const {
        
    }

    virtual size_t size() const override {
        return size;
    }

    virtual size_t free() override {
        delete[] entry;
    }

    virtual bool valid() const override {
        if(entry == nullptr || size <= 0)
            return false;
        else
            return true;
    }


};