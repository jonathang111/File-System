#pragma once
#include "MemInterface.h"
#include "../GeneralItems.h"
template <typename T>
class MemWrapper : public MemInterface<T> {
    private:
        Entry* entry;
        size_t size;
    public:
    explicit MemWrapper(size_t size){ //if overloading = op, cannot trip up with implicit conversion
        if(size > 0)
            entry = new Entry[size];
        else
            entry = nullptr;
    }

    ~MemWrapper(){delete[] entry;}

    T* get() const override {return entry;}

    T& operator*() const override {return *entry;}

    T* operator->() const override {return entry;}

    T& operator[](size_t index) const {
        
    }

    virtual size_t size() const override {return size;}

    virtual size_t free() override {delete[] entry;}

    virtual bool valid() const override {
        if(entry == nullptr || size <= 0)
            return false;
        else
            return true;
    }


};