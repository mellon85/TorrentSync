#include <sqlite3.h>
#include <torrentsync/dbm/Transaction.h>
#include <torrentsync/dbm/DatastoreImpl.h>
#include <torrentsync/dbm/util/Deleter.h>

namespace torrentsync
{
namespace dbm
{

Transaction::Transaction(DatastoreImpl& impl) : impl(impl)
{
    auto begin = impl.compile("BEGIN TRANSACTION");
    const int rc = sqlite3_step(begin.get());
    if (rc != SQLITE_OK)
    {
        // throw
    }
}

Transaction::~Transaction()
{
    auto end = impl.compile("ROLLBACK TRANSACTION");
    if (end.get() != nullptr)
    {
        const int rc = sqlite3_step(end.get());
        assert(rc == SQLITE_OK);
    }
}


} /* dbm */
} /* torrentsync */
