#pragma once
#include <map>
#include <string>
#include <any>

namespace GObject
{
    class Object
    {
    private:
        std::map<std::string, std::any> properties;

        void assert_property(std::string);

    public:
        Object(std::map<std::string, std::any>);

        template <typename T>
        T get_property(std::string name)
        {
            assert_property(name);
            return std::any_cast<T>(this->properties[name]);
        }
        void set_property(std::string name, std::any value)
        {
            assert_property(name);
            this->properties[name] = value;
        }
    };
}
