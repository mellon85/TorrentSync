#include <sqlite3.h>
#include <torrentsync/dbm/Transaction.h>
#include <torrentsync/dbm/DatastoreImpl.h>
#include <torrentsync/dbm/util/Deleter.h>
#include <iostream>
#include <torrentsync/utils/log/Logger.h>

namespace torrentsync
{
namespace dbm
{

Transaction::Transaction(DatastoreImpl& impl) : impl(impl), done(false)
{
    LOG(DEBUG, "DBM * Transaction Start");
    QueryStatus rc = impl.raw("BEGIN TRANSACTION");
    std::cout << rc << std::endl;
    if (rc != OK)
    {
        throw std::runtime_error("Couldn't start transaction");
        // throw
    }
}

Transaction::~Transaction()
{
    if (done)
        return;

    auto end = impl.raw("ROLLBACK TRANSACTION");
    if (end == OK)
    {
        LOG(DEBUG, "DBM * Transaction rolled back");
    }
    else {
        LOG(ERROR, "DBM * Unable to rollback transaction: "  << end);
    }
}

void Transaction::commit()
{
    assert(!done);

    auto commit = impl.raw("COMMIT");
    if (commit == OK)
    {
        LOG(DEBUG, "DBM * Transaction Commited");
    }
    else {
        LOG(ERROR, "DBM * Unable to commit transaction: "  << commit);
    }
}

bool Transaction::committed() const noexcept
{
    return done;
}

} /* dbm */
} /* torrentsync */
