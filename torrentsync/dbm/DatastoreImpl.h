#pragma once

#include <sqlite3.h>
#include <string>
#include <boost/filesystem.hpp>

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

private:
    sqlite3 *db;
};

} /* dbm */
} /* torrentsync */
