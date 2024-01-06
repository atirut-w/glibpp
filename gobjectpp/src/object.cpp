#include <gobjectpp/object.hpp>
#include <stdexcept>

using namespace GObject;

void Object::assert_property(std::string name)
{
    if (this->properties.find(name) == this->properties.end())
    {
        throw std::invalid_argument("Property `" + name + "` does not exist");
    }
}

Object::Object(std::map<std::string, std::any> properties)
{
    this->properties = properties;
}
