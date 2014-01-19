#include <torrentsync/dht/message/BEncodeDecoder.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

virtual void BEncodeDecoder::parseMessage()
{
    const char buff = stream.get(1);
    //! TODO check failbit

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
            
            if (structureStack.pop_back() == DICTIONARY)
                onDictionaryEnd();
            else
                onListEnd();
            break;

        default:
            if (structureStack.empty())
                throw BEncodeException("Malformed message - no structure found");

            if (structureStack.end()-1 == DICTIONARY)
            {
                const std::string key, value;
                key = readElement();
                value = readElement();
                onElement(key,value);
            }
            else
            {
                assert(structureStack.end()-1 == LIST);
                const std::string value;
                value = readElement();
                onElement(value);
            }
    }
}

std::string BEncodeDecoder::readElement()
{
    //! TODO check failbit
    int length;
    char *string;

    stream >> length;
    stream >> buff;
    if (buff != ':')
    {
        std::stringstream msg;
        msg << "Malformed message - expecting separator : but found ";
        msg << buff;
        throw BEncodeException(msg.str());
    }
    string = alloca(length+1);
    stream.get(string,length);
    string[length] = '\0';

    return string;
}

} // torrentsync
} // dht
} // message

