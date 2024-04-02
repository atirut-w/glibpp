#pragma once
#include <glib/global.hpp>

namespace GLib
{
    template <typename T>
    class Pointer
    {
    private:
        T *ptr = nullptr;
        gint *refs = nullptr;

        void unref()
        {
            if (ptr && --(*refs) == 0)
            {
                delete ptr;
                delete refs;
                ptr = nullptr;
                refs = nullptr;
            }
        }

    public:
        Pointer() : ptr(nullptr), refs(nullptr) {}
        Pointer(T *ptr)
        {
            this->ptr = ptr;
            if (this->ptr != nullptr)
            {
                refs = new gint(1);
            }
        }

        Pointer(const Pointer &other)
        {
            ptr = other.ptr;
            refs = other.refs;
            if (ptr != nullptr)
            {
                (*refs)++;
            }
        }

        Pointer &operator=(const Pointer &other)
        {
            if (this != &other)
            {
                unref();
                ptr = other.ptr;
                refs = other.refs;
                if (ptr != nullptr)
                {
                    (*refs)++;
                }
            }
            return *this;
        }

        Pointer(Pointer &&other)
        {
            ptr = other.ptr;
            refs = other.refs;
            other.ptr = nullptr;
            other.refs = nullptr;
        }

        Pointer &operator=(Pointer &&other)
        {
            if (this != &other)
            {
                unref();
                ptr = other.ptr;
                refs = other.refs;
                other.ptr = nullptr;
                other.refs = nullptr;
            }
            return *this;
        }

        ~Pointer()
        {
            unref();
        }

        operator bool() const { return ptr != nullptr; }
        T *operator->() const { return ptr; }
        T &operator*() const { return *ptr; }
    };

    class FileStream
    {
    private:
        FileStream() = default;
        void *stream = nullptr;

    public:
        static Pointer<FileStream> open(gint fd, string mode);
        static Pointer<FileStream> open(string path, string mode);
        ~FileStream();

        string gets(gchar *buffer, gsize len);
        int puts(string str);
    };
}
