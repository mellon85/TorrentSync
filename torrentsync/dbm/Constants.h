#pragma once

namespace torrentsync
{
namespace dbm
{

/**
 * Possible outcomes of a query
 */
enum QueryStatus {
    //! OK
    OK,
    //! The query should be retried at a later time
    RETRY,
    //! Query succesful and more data is available
    DATA_READY,
    //! Error executing the query
    ERROR
};

} /* dbm */
} /* torrentsync */
