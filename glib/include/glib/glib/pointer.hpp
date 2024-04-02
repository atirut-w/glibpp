#pragma once
#include <glib/primitives.hpp>

namespace GLib
{
    // Encapsulates owned (ref-counted) pointer
    template <typename T>
    class owned
    {
    private:
        T *ptr = nullptr;
        gint *refs = nullptr;

        void cleanup()
        {
            if (ptr != nullptr && --(*refs) == 0)
            {
                delete ptr;
                delete refs;
                ptr = nullptr;
                refs = nullptr;
            }
        }
        
    public:
        owned() : ptr(nullptr), refs(nullptr) {}
        owned(T *ptr)
        {
            this->ptr = ptr;
            if (ptr != nullptr)
                refs = new gint(1);
        }

        owned(const owned &other)
        {
            ptr = other.ptr;
            refs = other.refs;
            if (ptr != nullptr)
                (*refs)++;
        }

        owned &operator=(const owned &other)
        {
            if (this != &other)
            {
                cleanup();
                ptr = other.ptr;
                refs = other.refs;
                if (ptr != nullptr)
                    (*refs)++;
            }
            return *this;
        }

        owned(owned &&other)
        {
            ptr = other.ptr;
            refs = other.refs;
            other.ptr = nullptr;
            other.refs = nullptr;
        }

        void operator=(owned &&other)
        {
            cleanup();
            ptr = other.ptr;
            refs = other.refs;
            other.ptr = nullptr;
            other.refs = nullptr;
        }

        ~owned() { cleanup(); }

        operator gboolean() const { return ptr != nullptr; }
        T *operator->() const { return ptr; }
        T &operator*() const { return *ptr; }
    };
}
