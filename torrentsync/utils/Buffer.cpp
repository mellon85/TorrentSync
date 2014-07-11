#include <torrentsync/utils/Buffer.h>

std::ostream& operator<<( std::ostream& stream, const pretty_print& buff )
{
    const auto flags = stream.flags();
    stream << std::hex;
    for( auto it = buff._buff.cbegin(); it != buff._buff.cend(); ++it )
    {
        if ( std::isprint(*it))
        {
            stream << *it;
        }
        else if (*it != 0)
        {
            stream << "0x" << (static_cast<uint16_t>(*it) & 0x00FF);
        }
        else
        {
            break;
        }
    }
    stream.setf(flags);
    return stream;
}
