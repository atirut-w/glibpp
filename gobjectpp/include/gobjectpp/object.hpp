#pragma once
#include <map>
#include <string>
#include <any>
#include <stdexcept>

namespace GObject
{
    class Object
    {
    protected:
        std::map<std::string, std::any> properties;

    private:
        template <typename T>
        void assert_property(std::string name)
        {
            if (properties.find(name) == properties.end())
            {
                throw std::invalid_argument("Property " + name + " does not exist");
            }
            if (properties[name].type() != typeid(T))
            {
                throw std::invalid_argument("Property type mismatch");
            }
        }

    public:
        Object(std::map<std::string, std::any>);

        template <typename T>
        T get_property(std::string name)
        {
            assert_property<T>(name);
            return std::any_cast<T>(properties[name]);
        }

        template <typename T>
        void set_property(std::string name, T value)
        {
            assert_property<T>(name);
            this->properties[name] = value;
        }
    };
}
