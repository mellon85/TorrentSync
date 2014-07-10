#pragma once

#include <functional>

namespace torrentsync
{
namespace utils
{

/*! Finally can be used to execute specific lambda/functions at the end
    of the current scope.
    The passed function will be called when the object will be destroyed.
 */
class Finally
{
public:
    typedef std::function<void ()> clean_handler;

    Finally( clean_handler handler ) noexcept : _handler(handler) {}

    ~Finally() { _handler(); }

private:
    clean_handler _handler;
};

}; // utils
}; // torrentsync
