#pragma once

#include <string>

#define TEST_LOOP_COUNT 100

inline std::string generateRandomAddress( const std::string& prefix = std::string())
{
    std::string data;
    data.reserve(40);
	data.append(prefix);
	assert(prefix.length() < 40);
    for( int j = prefix.length(); j < 40; ++j )
    {
        switch (rand()%3)
        {
            case 0:
                data += static_cast<char>(rand()%10 + 48); // 0-9
                break;
            case 1:
                data += static_cast<char>(rand()%6 + 65); // A-F
                break;
            case 2:
                data += static_cast<char>(rand()%6 + 97); // a-f
                break;
        }
    }
    return data;
}
