#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <cassert>

#include <boost/lexical_cast.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{

void BEncodeDecoder::parseMessage( std::istream& stream )
{
    bool parseEnded = false;
    size_t listCounter = 0;
    data.clear();
    structureStack.clear();
    structureStack.reserve(2); // should be enough for every KRPC
    std::string last_token;

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
                    const bool allDigits =
                            std::all_of( structureStack.back().first.begin(),
                                         structureStack.back().first.end(),
                                         isdigit);
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
                    std::string key;
                    torrentsync::utils::Buffer value;
                    key = readElement(stream);

                    // peek if it's a 'l' or a 'd' (substructure)
                    switch (stream.peek())
                    {
                    case 'l':
                    case 'd':
                        last_token = key; // structure itself
                        break;
                    default:
                        value = readValue(stream);

                        std::for_each( structureStack.begin(),structureStack.end(), [&] (const structureStackE& path)
                        {
                            buffer += path.first;
                            if (buffer.size() > 0)
                                buffer += '/';
                        });

                        buffer += key;
                        data.insert(std::make_pair(buffer,value));
                        break;
                    }
                }
                else
                {
                    assert(structureStack.back().second == LIST);
                    torrentsync::utils::Buffer value;
                    value = readValue(stream);

                    std::for_each( structureStack.begin(),structureStack.end(), [&] (const structureStackE& path)
                    {
                        buffer += path.first;
                        if (buffer.size() > 0)
                            buffer += '/';
                    });

                    buffer += boost::lexical_cast<std::string>(listCounter);
                    ++listCounter;
                    data.insert(std::make_pair(buffer,value));
                }
        }
    }

    if (!parseEnded)
    {
        throw BEncodeException("Message could not be parsed correctly");
    }
    else
    {
        structureStack.clear(); // release remaining used memory
    }
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

torrentsync::utils::Buffer BEncodeDecoder::readValue( std::istream& stream )
{
    
    int length;

    stream >> length;
    const char separator = stream.get();

    torrentsync::utils::Buffer buff(length);

    if (!stream.good())
        throw BEncodeException("Error in stream while reading an element");

    if (separator != ':')
    {
        std::stringstream msg;
        msg << "Malformed message - expecting separator : but found " << separator;
        throw BEncodeException(msg.str());
    }

    {
        assert(buff.get());
        stream.read(buff.get(),length);
        return buff;
    }
}

} // torrentsync
} // dht
} // message

