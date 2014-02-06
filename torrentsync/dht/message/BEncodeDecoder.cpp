#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <cassert>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{

void BEncodeDecoder::parseMessage( std::istream& stream )
{
    bool parseEnded = false;
    listCounter = 0;
    data.clear();
    structureStack.clear();

    while ( !stream.eof() && !parseEnded )
    {
        if (!stream.good())
            throw BEncodeException("Error in stream while reading a char");

        switch (stream.peek())
        {
            case 'd':
                stream.ignore();
                structureStack.push_back(std::make_pair(last_token,DICTIONARY));
                break;

            case 'l':
                stream.ignore();
                if (listCounter > 0)
                {
                    last_token = boost::lexical_cast<std::string>(listCounter);
                }
                listCounter = 0;
                structureStack.push_back(std::make_pair(last_token,LIST));
                break;

            case 'e':
                stream.ignore();
                if (structureStack.size() <= 0)
                    throw BEncodeException("Malformed message - end not in a structure");
                
                listCounter = 0;
                if (structureStack.size() > 0 && structureStack.back().second == LIST)
                {
#ifdef HAS_Cxx11
                    const bool allDigits = std::all( structureStack.back().second.begin(),
                                                     structureStack.back().second.end(),
                                                     isdigit);
#else
                    bool allDigits = true;
                    BOOST_FOREACH( const char c, structureStack.back().first)
                    {
                        if (!isdigit(c))
                        {
                            allDigits = false;
                            break;
                        }
                    }
#endif
                    if (allDigits)
                    {
                        try
                        {
                            listCounter = boost::lexical_cast<int>(structureStack.back().first)+1;
                        } catch (...) { listCounter = 0; }
                    }
                }
                structureStack.pop_back();
                if (structureStack.size() == 0)
                {
                    parseEnded = true;
                }
                break;

            default:
                if (structureStack.empty())
                    throw BEncodeException("Malformed message - no structure found");
                std::string buffer;
                buffer.reserve(16);

                if (structureStack.back().second == DICTIONARY)
                {
                    std::string key, value;
                    key = readElement(stream);

                    // peek if it's a 'l' or a 'd' (substructure)
                    switch (stream.peek())
                    {
                    case 'l':
                    case 'd':
                        last_token = key; // structure itself
                        break;
                    default:
                        value = readElement(stream);

                        BOOST_FOREACH( const structureStackE& path, structureStack )
                        {
                            buffer += path.first;
                            buffer += '/';
                        }

                        buffer += key;
                        data.insert(std::make_pair(buffer,value));
                        break;
                    }
                }
                else
                {
                    assert(structureStack.back().second == LIST);
                    std::string value;
                    value = readElement(stream);

                    BOOST_FOREACH( const structureStackE& path, structureStack )
                    {
                        buffer += path.first;
                        buffer += '/';
                    }

                    buffer += boost::lexical_cast<std::string>(listCounter);
                    ++listCounter;
                    data.insert(std::make_pair(buffer,value));
                }
        }
    }

    if (!parseEnded)
        throw BEncodeException("Message could not be parsed correctly");
}

std::string BEncodeDecoder::readElement( std::istream& stream )
{
    
    int length;

    stream >> length;
    const char buff = stream.get();

    if (!stream.good())
        throw BEncodeException("Error in stream while reading an element");

    if (buff != ':')
    {
        std::stringstream msg;
        msg << "Malformed message - expecting separator : but found " << buff;
        throw BEncodeException(msg.str());
    }

    {
        char string[length+1];
        stream.read(string,length);
        string[length] = '\0';
        return string;
    }
}

} // torrentsync
} // dht
} // message

