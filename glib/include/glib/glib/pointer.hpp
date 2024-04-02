#pragma once
#include <glib/primitives.hpp>

namespace GLib
{
    // // Encapsulates unowned pointer
    // template <typename T>
    // class unowned
    // {
    // private:
    //     T *ptr = nullptr;
    //     gint *refs = nullptr;

    //     void cleanup()
    //     {
    //         if (ptr != nullptr && --(*refs) == 0)
    //         {
    //             delete ptr;
    //             delete refs;
    //             ptr = nullptr;
    //             refs = nullptr;
    //         }
    //     }
        
    // public:
    //     unowned() : ptr(nullptr), refs(nullptr) {}
    //     unowned(T *ptr)
    //     {
    //         this->ptr = ptr;
    //         if (ptr != nullptr)
    //             refs = new gint(1);
    //     }

    //     unowned(const unowned &other)
    //     {
    //         ptr = other.ptr;
    //         refs = other.refs;
    //         if (ptr != nullptr)
    //             (*refs)++;
    //     }

    //     unowned &operator=(const unowned &other)
    //     {
    //         if (this != &other)
    //         {
    //             cleanup();
    //             ptr = other.ptr;
    //             refs = other.refs;
    //             if (ptr != nullptr)
    //                 (*refs)++;
    //         }
    //         return *this;
    //     }

    //     unowned(unowned &&other)
    //     {
    //         ptr = other.ptr;
    //         refs = other.refs;
    //         other.ptr = nullptr;
    //         other.refs = nullptr;
    //     }

    //     void operator=(unowned &&other)
    //     {
    //         cleanup();
    //         ptr = other.ptr;
    //         refs = other.refs;
    //         other.ptr = nullptr;
    //         other.refs = nullptr;
    //     }

    //     ~unowned() { cleanup(); }

    //     operator gboolean() const { return ptr != nullptr; }
    //     T *operator->() const { return ptr; }
    //     T &operator*() const { return *ptr; }
    // };
    
    // Encapsulates owned pointer
    template <typename T>
    class owned
    {
    private:
        T *ptr = nullptr;

        void cleanup()
        {
            if (ptr != nullptr)
                delete ptr;
        }

    public:
        owned() = default;
        owned(T *ptr) : ptr(ptr) {}

        owned(const owned &) = delete;
        owned &operator=(const owned &) = delete;

        owned(owned &&other)
        {
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        void operator=(owned &&other)
        {
            cleanup();
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        ~owned() { cleanup(); }

        operator gboolean() const { return ptr != nullptr; }
        T *operator->() const { return ptr; }
        T &operator*() const { return *ptr; }
    };
}
