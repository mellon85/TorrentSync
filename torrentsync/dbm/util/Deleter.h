#pragma once

#include <memory>

class sqlite3_stmt;

namespace torrentsync
{
namespace dbm
{
namespace util
{

class Deleter
{
public:
    void operator()(sqlite3_stmt*);
};

} /* util */

typedef std::unique_ptr<sqlite3_stmt, util::Deleter> Query;

} /* dbm */
} /* torrentsync */
