#pragma once
#include <glib/global.hpp>

namespace GLib
{
    template <typename T>
    class Pointer
    {
    private:
        T *ptr = nullptr;

        void cleanup()
        {
            if (ptr)
                delete ptr;
        }

    public:
        Pointer() = default;
        Pointer(T *ptr) : ptr(ptr) {}

        // "There is no copying in Ba Sing Se"
        Pointer(const Pointer &other) = delete;
        Pointer &operator=(const Pointer &other) = delete;

        Pointer(Pointer &&other)
        {
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        void operator=(Pointer &&other)
        {
            cleanup();
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        T *operator->() { return ptr; }
        T &operator*() { return *ptr; }

        ~Pointer() { cleanup(); }
    };

    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

    public:
        static FileStream *open(gint fd, string mode);
        static FileStream *open(string path, string mode);
        ~FileStream();

        int puts(string str);
    };
}
