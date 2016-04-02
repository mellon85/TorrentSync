#include <sqlite3.h>
#include <torrentsync/dbm/Transaction.h>
#include <torrentsync/dbm/DatastoreImpl.h>
#include <torrentsync/dbm/util/Deleter.h>

namespace torrentsync
{
namespace dbm
{

Transaction::Transaction(DatastoreImpl& impl) : impl(impl), committed(false)
{
    auto begin = impl.compile("BEGIN TRANSACTION");
    const int rc = impl.execute(begin);
    if (rc != SQLITE_OK)
    {
        // throw
    }
}

Transaction::~Transaction()
{
    if (committed)
        return;

    auto end = impl.compile("ROLLBACK TRANSACTION");
    if (end)
    {
        const int rc = impl.execute(end);
        assert(rc == SQLITE_OK);
    }
}

void Transaction::commit()
{
    auto commit = impl.compile("COMMIT");
    if (commit)
    {
        const int rc = impl.execute(commit);
        assert(rc == SQLITE_OK);
    }
}


} /* dbm */
} /* torrentsync */
