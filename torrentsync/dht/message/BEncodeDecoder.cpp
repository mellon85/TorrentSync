#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <cassert>

#include <boost/lexical_cast.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include <iostream>

namespace torrentsync
{
namespace dht
{
namespace message
{

using namespace torrentsync;

void BEncodeDecoder::parseMessage( std::istream& stream )
{
    bool parseEnded = false;
    size_t listCounter = 0;
    data.clear();
    structureStack.clear();
    structureStack.reserve(2); // should be enough for every KRPC
    utils::Buffer last_token;

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
                    last_token = utils::makeBuffer(listCounter);

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
                            const utils::Buffer& data = structureStack.back().first;
                            listCounter = boost::lexical_cast<int>(std::string(data.cbegin(),data.cend()))+1;
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
                utils::Buffer buffer;
                buffer.reserve(16);

                if (structureStack.back().second == DICTIONARY)
                {
                    utils::Buffer key = readElement(stream);
                    utils::Buffer value;

                    // peek if it's a 'l' or a 'd' (substructure)
                    switch (stream.peek())
                    {
                    case 'l':
                    case 'd':
                        last_token = key; // structure itself
                        break;
                    default:
                        value = readValue(stream);

                        buffer = appendPath();
                        buffer += key;
                        data.insert(std::make_pair(buffer,value));
                        break;
                    }
                }
                else
                {
                    assert(structureStack.back().second == LIST);
                    utils::Buffer value = readValue(stream);

                    buffer = appendPath();

                    auto counter = utils::makeBuffer(listCounter);
                    buffer += counter;
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

utils::Buffer BEncodeDecoder::appendPath()
{
    utils::Buffer buff;
    buff.reserve(16);
    std::for_each( structureStack.begin(),structureStack.end(), [&] (const structureStackE& path)
    {
        //buff.insert(buff.end(),path.first.cbegin(),path.first.cend());
        buff += path.first;
        if (buff.size() > 0)
            buff.push_back('/');
    });
    return buff;
}

utils::Buffer BEncodeDecoder::readElement( std::istream& stream )
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
        return utils::makeBuffer(string);
    }
}

utils::Buffer BEncodeDecoder::readValue( std::istream& stream )
{
    if (stream.peek() == 'i')
    {
        stream.ignore();

        // encoded integer value
        uint64_t value;
        stream >> value;

        char delimiter;
        stream >> delimiter;
        if (delimiter != 'e')
        {
            throw BEncodeException("Missing integer terminator");
        }
        return utils::makeBuffer(value);
    }
    else
    {
        size_t length;

        stream >> length;
        const char separator = stream.get();

        if (!stream.good())
            throw BEncodeException("Error in stream while reading an element");

        if (separator != ':')
        {
            std::stringstream msg;
            msg << "Malformed message - expecting separator : but found " << separator;
            throw BEncodeException(msg.str());
        }

        utils::Buffer buffer;
        buffer.reserve(length);
        for( size_t __i = 0; __i < length; ++__i )
        {
            buffer.push_back(stream.get());
        }
        return buffer;
    }
}

} // torrentsync
} // dht
} // message

