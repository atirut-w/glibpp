#include <gobjectpp/object.hpp>

using namespace GObject;

Object::Object(std::map<std::string, std::any> properties)
{
    this->properties = properties;
}
