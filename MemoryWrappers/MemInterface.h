#pragma once

template<typename T>
class MemInterface{
    public:
        virtual ~MemInterface() = default;

        virtual T* get() const = 0;

        virtual T& operator*() const = 0;
        virtual T* operator->() const = 0;

        virtual size_t size() const = 0;

        virtual size_t free() = 0;

        virtual bool valid() const = 0;
};