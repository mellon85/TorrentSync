#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <torrentsync/dbm/Transaction.h>
#include <torrentsync/dbm/util/Deleter.h>

#include <sqlite3.h>

class sqlite3;
class sqlite3_stmt;

namespace torrentsync
{
namespace dbm
{


class DatastoreImpl
{
public:
    /** Opens the database connection
     * \param path the database path
     * \param readOnly specify if the database has to be opened read only.
     * \note may throw a runtime exception if the database couldn't be opened
     */
    DatastoreImpl(const boost::filesystem::path& path, bool readOnly);

    //! The destructor will close the database and synchronize it to disk
    ~DatastoreImpl();

    DatastoreImpl& operator=(const DatastoreImpl&) = delete;
    DatastoreImpl(const DatastoreImpl&) = delete;

    DatastoreImpl& operator=(DatastoreImpl&&) = default;
    DatastoreImpl(DatastoreImpl&&) = default;

    sql_compiled compile(const std::string& sql);

private:
    sqlite3 *db;
};

} /* dbm */
} /* torrentsync */
