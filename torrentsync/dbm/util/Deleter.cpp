#include <torrentsync/dbm/util/Deleter.h>
#include <sqlite3.h>

namespace torrentsync
{
namespace dbm
{
namespace util
{

void Deleter::operator()(sqlite3_stmt* stmt)
{
    sqlite3_finalize(stmt);
}

} /* util */
} /* dbm */
} /* torrentsync */
