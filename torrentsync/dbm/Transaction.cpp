#include <sqlite3.h>
#include <torrentsync/dbm/Transaction.h>
#include <torrentsync/dbm/DatastoreImpl.h>
#include <torrentsync/dbm/util/Deleter.h>

namespace torrentsync
{
namespace dbm
{

Transaction::Transaction(DatastoreImpl& impl) : impl(impl), done(false)
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
    if (done)
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
    assert(!done);

    auto commit = impl.compile("COMMIT");
    if (commit)
    {
        const int rc = impl.execute(commit);
        assert(rc == SQLITE_OK);
        done = true;
    }
}

bool Transaction::committed() const noexcept
{
    return done;
}

} /* dbm */
} /* torrentsync */
