#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <cassert>
#include <boost/scoped_ptr.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{

void BEncodeDecoder::parseMessage( std::istream& stream )
{
    bool parseEnded = false;
    while ( !stream.eof() && !parseEnded )
    {
        char buff;
        stream >> buff;
        if (!stream.good())
            throw BEncodeException("Error in stream while reading a char");

        switch (buff)
        {
            case 'd':
                structureStack.push_back(DICTIONARY);
                onDictionaryStart();
                break;

            case 'l':
                structureStack.push_back(LIST);
                onListStart();
                break;

            case 'e':
                if (structureStack.size() <= 0)
                    throw BEncodeException("Malformed message - end not in a structure");
                
                if (structureStack.back() == DICTIONARY)
                    onDictionaryEnd();
                else
                    onListEnd();
                structureStack.pop_back();
                if (structureStack.size() == 0)
                {
                    parseEnded = true;
                }
                break;

            default:
                if (structureStack.empty())
                    throw BEncodeException("Malformed message - no structure found");

                stream.putback(buff);
                if (structureStack.back() == DICTIONARY)
                {
                    std::string key, value;
                    key = readElement(stream);

                    // peek if it's a 'l' or a 'd' (substructure)
                    buff = stream.peek();
                    switch (buff)
                    {
                    case 'l':
                    case 'd':
                        onElement(key); // the next loop will take care of the
                                        // structure itself
                        break;
                    default:
                        value = readElement(stream);
                        onElement(key,value);
                        break;
                    }
                }
                else
                {
                    assert(structureStack.back() == LIST);
                    std::string value;
                    value = readElement(stream);
                    onElement(value);
                }
        }
    }

    if (!parseEnded)
        throw BEncodeException("Message could not be parsed correctly");
}

std::string BEncodeDecoder::readElement( std::istream& stream )
{
    //! TODO check failbit
    char buff;
    int length;

    //! TODO should throw not assert
    assert( stream.good() );

    stream >> length;

    stream >> buff;
    if (buff != ':')
    {
        std::stringstream msg;
        msg << "Malformed message - expecting separator : but found ";
        msg << buff;
        throw BEncodeException(msg.str());
    }

    {
        char string[length+1];
        stream.read(string,length);
        string[length] = '\0';
        return string; // copyed into a std::string object
    }
}

} // torrentsync
} // dht
} // message

