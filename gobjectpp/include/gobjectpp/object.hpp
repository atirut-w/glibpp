#pragma once
#include <map>
#include <string>
#include <any>
#include <stdexcept>

namespace GObject
{
    class Object
    {
    private:
        std::map<std::string, std::any> properties;

    public:
        Object(std::map<std::string, std::any>);

        template <typename T>
        T get_property(std::string name)
        {
            if (this->properties.find(name) == this->properties.end())
            {
                throw std::invalid_argument("Property `" + name + "` not found");
            }
            return std::any_cast<T>(this->properties[name]);
        }
        void set_property(std::string name, std::any value)
        {
            if (this->properties.find(name) == this->properties.end())
            {
                throw std::invalid_argument("Property `" + name + "` not found");
            }
            this->properties[name] = value;
        }
    };
}
