#include <torrentsync/dbm/DatastoreImpl.h>
#include <system_error>
#include <cassert>
#include <thread>
#include <sqlite3.h>

namespace torrentsync
{
namespace dbm
{

DatastoreImpl::~DatastoreImpl()
{
#ifdef sqlite3_close_v2
    sqlite3_close_v2(db);
#else
    int rc = sqlite3_close(db);
    assert(rc == SQLITE_OK);
#endif
}

DatastoreImpl::DatastoreImpl(const boost::filesystem::path& path, bool readOnly)
{
    int flags = SQLITE_OPEN_CREATE;
    if (readOnly)
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

torrentsync::dbm::sql_compiled
DatastoreImpl::compile(const std::string& sql)
{
    sqlite3_stmt *stmt;

    const int rc = sqlite3_prepare_v2(db,
            sql.c_str(), sql.size(),
            &stmt, NULL);

    if (rc != SQLITE_OK)
        throw std::runtime_error("Failed to compile: "+sql);

    return sql_compiled(stmt);
}

} /* dbm */
} /* torrentsync */
