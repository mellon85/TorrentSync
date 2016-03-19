#include <torrentsync/dbm/DatastoreImpl.h>
#include <system_error>

namespace torrentsync
{
namespace dbm
{

DatastoreImpl::~DatastoreImpl()
{
    sqlite3_close_v2(db);
}

DatastoreImpl::DatastoreImpl(const std::string& path, bool readOnly)
{
    int flags = SQLITE_OPEN_CREATE;
    if (!readOnly)
        flags |= SQLITE_OPEN_READONLY;
    else
        flags |= SQLITE_OPEN_READWRITE;

    int rc = sqlite3_open_v2(path.c_str(), &db, flags, NULL);
    if (rc != SQLITE_OK)
    {
        throw std::system_error(rc, std::generic_category(),
                "Failed to open the database");
    }
}

} /* dbm */
} /* torrentsync */
