#pragma once

#include <boost/optional.hpp>
#include <type_traits>

namespace torrentsync
{
namespace dht
{
class NodeData;
}

namespace utils
{

using namespace boost::mpl;

/**
 * Linearly returns the elements between begin and end iterator.
 * Useful to avoid creating template classes/methods just to pass a pair of
 * iterators of some type.
 */
template <typename Iterator, typename OutputType = void>
class Yield
{
public:
    typedef Iterator                            iterator_type;
    typedef typename iterator_type::value_type  in_type;

    typedef typename std::conditional<
        std::is_same<OutputType,void>::value,
            in_type, OutputType>::type         value_type;
    
    typedef typename boost::optional<value_type>         return_type;
    typedef typename std::function<return_type()>      function_type;
    
    static_assert( std::is_convertible<in_type,value_type>::value, "input type not convertible to output type");

    Yield( Iterator begin, Iterator end ) : _begin(begin), _end(end) {}
    
    return_type operator()()
    {
        return _begin != _end ? return_type(static_cast<value_type>(*_begin++)) : return_type();
    }
    
    function_type function()
    {
        return [&] { return (*this)(); };
    }
    
private:
    iterator_type _begin, _end;
};

template <typename OutputType = void, typename Iterator>
auto makeYield( Iterator begin, Iterator end ) -> Yield<Iterator,OutputType>
{
    return Yield<Iterator,OutputType>(begin,end);
}

}; // utils
}; // torrentsync
