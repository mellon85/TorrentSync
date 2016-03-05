#pragma once

#include <kchashdb.h>

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
     *        As per KyotoCabinet implementation only one writer can keep the
     *        database opened at any one time.
     *        To share it the database should not be kept opened but only opened
     *        when needed and then closed.
     */
    DatastoreImpl(const std::string& path, bool readOnly);

    //! The destructor will close the database and synchronize it to disk
    ~DatastoreImpl();

    DatastoreImpl& operator=(const DatastoreImpl&) = delete;
    DatastoreImpl(const DatastoreImpl&) = delete;

    DatastoreImpl& operator=(DatastoreImpl&&) = default;
    DatastoreImpl(DatastoreImpl&&) = default;

private:
    kyotocabinet::TreeDB db;
};

} /* dbm */
} /* torrentsync */
