#pragma once

#include <string>
#include <memory>
#include <torrentsync/dbm/DatastoreImpl.h>

#include <boost/filesystem.hpp>

namespace torrentsync
{
namespace dbm
{

/**
 * Forward delcaration to hide the database implementation details
 */
class DatastoreImpl;

/**
 * Interface to the application data store
 *
 * Current implementation is based over Kyoto Cabinet
 */
class Datastore
{
public:

    /**
     * Construtor of the database interface
     * \param path The file path of the database
     */
    Datastore(const boost::filesystem::path& path,
              bool readOnly = false);

    /**
     * Copying not allowed
     */
    Datastore(const Datastore&) = delete;

    /**
     * Destructor
     */
    ~Datastore();

    /**
     * Copying not allowed
     */
    void operator=(const Datastore&) = delete;

    /**
     * Moving is allowed
     */
    Datastore(Datastore&&) = default;

    /**
     * Moving is allowed
     */
    Datastore& operator=(Datastore&&) = default;

    // @TODO operations
    // add file
    // add directory
    // create iterator class to have a rolling synchronization message streaming
    //  from the database

private:
    // Datastore implementation pointer
    std::unique_ptr<DatastoreImpl> impl;
};

} /* dbm */
} /* torrentsync */
